#include <minecraft/BinaryDataOutput.h>
#include <minecraft/BinaryStream.h>
#include <minecraft/Block.h>
#include <minecraft/BlockLegacy.h>
#include <minecraft/BlockPalette.h>
#include <minecraft/BlockTypeRegistry.h>
#include <minecraft/CompoundTag.h>
#include <minecraft/ItemRegistry.h>
#include <minecraft/Level.h>
#include <minecraft/LevelSoundEventMap.h>
#include <minecraft/Minecraft.h>
#include <minecraft/NbtIo.h>
#include <minecraft/ParticleTypeMap.h>
#include <minecraft/ServerInstance.h>
#include <minecraft/Biome.h>
#include <minecraft/BiomeRegistry.h>

#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <json.hpp>


extern "C" void _ZN16BinaryDataOutputC2ER12BinaryStream(BinaryDataOutput*, BinaryStream*);

void generate_r12_to_current_block_map(ServerInstance *serverInstance) {
	auto palette = serverInstance->getMinecraft()->getLevel()->getBlockPalette();

	std::filesystem::path inputPath{"input_files/r12_block_states.json"};
	if (!std::filesystem::exists(inputPath)) {
		std::cerr << "Input file " << inputPath << " not found, r12_to_current_block_map won't be generated!" << std::endl;
		return;
	}
	std::ifstream input(inputPath);

	auto stream = new BinaryStream();

	BinaryDataOutput dataOutput;
	_ZN16BinaryDataOutputC2ER12BinaryStream(&dataOutput, stream);

	auto json = nlohmann::json::object();
	input >> json;

	int count = 0;
	for (auto &object : json["minecraft"].items()) {
		const auto &name = "minecraft:" + object.key();
		auto blockLegacy = palette->getBlockLegacy(name);
		if (blockLegacy == nullptr) {
			continue;
		}
		for (auto &it : object.value()) {
			auto state = it.get<unsigned short>();

			if (name == "minecraft:cocoa" && state >= 12) {
//				continue;
			}

//			auto block = blockLegacy->getBlockStateFromLegacyData(state);
			auto block = blockLegacy->getStateFromLegacyData(state);

			stream->writeUnsignedVarInt(name.length());
			stream->write(name.c_str(), name.length());
			stream->writeUnsignedShort(state);

			std::cout << "enter" << std::endl;
			NbtIo::write(&block->tag, dataOutput);
			std::cout << "exit" << std::endl;
			count++;
//			stream->writeType(block->tag);
		}
	}

	std::cout << "writing file containing " << std::to_string(count) << " mappings" << std::endl;
	std::ofstream output("mapping_files/r12_to_current_block_map.bin");
	output << stream->buffer;
	std::cout << "done writing file" << std::endl;
	output.close();
	delete stream;
	std::cout << "Generated R12 block state mapping table" << std::endl;
}

static std::string slurp(std::ifstream& in) {
	std::ostringstream sstr;
	sstr << in.rdbuf();
	return sstr.str();
}

static void generate_old_to_current_palette_map(ServerInstance *serverInstance) {
	auto palette = serverInstance->getMinecraft()->getLevel()->getBlockPalette();

	unsigned int generated = 0;

	std::filesystem::path oldBlockPalettesPath{"input_files/old_block_palettes"};
	if (!std::filesystem::exists(oldBlockPalettesPath)) {
		std::cerr << "Input path " << oldBlockPalettesPath << " does not exist, no block maps will be generated!" << std::endl;
		return;
	}

	std::filesystem::create_directory("mapping_files/old_palette_mappings");

	for(auto const& file : std::filesystem::directory_iterator{oldBlockPalettesPath}){
		std::ifstream infile(file.path());
		auto versionName = file.path().stem().string();
		std::ofstream mapping_file("mapping_files/old_palette_mappings/" + versionName + "_to_current_block_map.bin");

		auto input = new ReadOnlyBinaryStream(slurp(infile));
		auto output = new BinaryStream();

		auto length = input->buffer.length();

		while(input->offset < length){
			CompoundTag state = input->getType<CompoundTag>();

			const Block* block = palette->getBlock(state);

			//TODO: compare and don't write if the states are the same
			//right now it's easier to do this outside of the mod
			output->writeType(state);
			output->writeType(block->tag);
		}

		mapping_file << output->buffer;
		mapping_file.close();
		delete input;
		delete output;

		std::cout << "Generated mapping table for " << versionName << std::endl;
		generated++;
	}

	std::cout << "Generated " << std::to_string(generated) << " block state mapping tables" << std::endl;
}


void generate_palette(ServerInstance *serverInstance) {
	auto palette = serverInstance->getMinecraft()->getLevel()->getBlockPalette();
	unsigned int numStates = palette->getNumBlockRuntimeIds();
	std::cout << "Number of blockstates: " << numStates << std::endl;

	auto paletteStream = new BinaryStream();
	BinaryDataOutput dataOutput;
	_ZN16BinaryDataOutputC2ER12BinaryStream(&dataOutput, paletteStream);

//	BinaryDataOutput dataOutput(*paletteStream);
	for (unsigned int i = 0; i < numStates; i++) {
		auto state = palette->getBlock(i);
		NbtIo::write(&state->tag, dataOutput);
//		paletteStream->writeType(state->tag);
	}

	std::ofstream paletteOutput("mapping_files/canonical_block_states.nbt");
	paletteOutput << paletteStream->buffer;
	paletteOutput.close();
	delete paletteStream;
	std::cout << "Generated block palette" << std::endl;
}

static void generate_hardness_table(ServerInstance *serverInstance) {
	auto palette = serverInstance->getMinecraft()->getLevel()->getBlockPalette();
	unsigned int numStates = palette->getNumBlockRuntimeIds();

	auto table = nlohmann::json::object();

	BlockTypeRegistry::forEachBlock([&table] (const BlockLegacy & blockLegacy)->bool {
		auto name = blockLegacy.getFullName();
		table[name] = blockLegacy.getDestroySpeed();
		return true;
	});

	std::ofstream output("mapping_files/hardness_table.json");
	output << std::setw(4) << table << std::endl;
	output.close();
	std::cout << "Generated hardness table" << std::endl;
}

void generate_item_mapping() {
	auto registry = static_cast<ItemRegistry*>(ItemRegistry::mItemRegistry);

	auto list = nlohmann::json::object();
	std::filesystem::path listFilePath{"input_files/item_id_map.json"};
	if (!std::filesystem::exists(listFilePath)) {
		std::cerr << "Input file " << listFilePath << " not found, item mapping table won't be generated!" << std::endl;
		return;
	}
	std::ifstream listFile(listFilePath);

	listFile >> list;

	auto simple = nlohmann::json::object();
	auto complex = nlohmann::json::object();

	for(auto it = list.begin(); it != list.end(); it++){
		auto key = it.key();
		auto firstMapped = registry->getNameFromAlias(key, 0);

		auto metaMap = nlohmann::json::object();
		for(auto meta = 1; meta < 300; meta++){
			auto mapped = registry->getNameFromAlias(key, meta);
			if(key != mapped.first.str && mapped.first.str != firstMapped.first.str){
				metaMap[std::to_string(meta)] = mapped.first.str;
				assert(mapped.second == 0);
			}
		}
		if(metaMap.size() > 0){
			if(key != firstMapped.first.str){
				metaMap["0"] = firstMapped.first.str;
			}
			complex[key] = metaMap;
		}else if(key != firstMapped.first.str){
			simple[key] = firstMapped.first.str;
		}
	}

	auto generated = nlohmann::json::object();
	generated["simple"] = simple;
	generated["complex"] = complex;
	std::ofstream result("mapping_files/r16_to_current_item_map.json");
	result << std::setw(4) << generated << std::endl;
	result.close();
	std::cout << "Generated R16U1 item mapping table" << std::endl;
}

void generate_biome_mapping(ServerInstance *server) {
	auto registry = server->getMinecraft()->getLevel()->getBiomeRegistry();

	auto map = nlohmann::json::object();

	registry->forEachBiome([&map] (Biome &biome) {
		auto id = biome.biomeId;
		map[biome.name] = id;
	});

	std::ofstream result("mapping_files/biome_id_map.json");
	result << std::setw(4) << map << std::endl;
	result.close();
	std::cout << "Generated biome ID mapping table" << std::endl;
}

static void generate_level_sound_mapping() {
	auto map = nlohmann::json::object();

	bool found_113 = false;
	for(int i = 0; i < 1000; i++) {
		auto name = LevelSoundEventMap::getName((LevelSoundEvent) i);
		if (name != "undefined") {
			map[name] = i;
			if (i == 113) {
				found_113 = true;
				std::cout << "WARNING: STOP_RECORD(113) has changed, and it won't appear in the output!" << std::endl;
			}
		}
	}
	if (!found_113) {
		map["stop_record"] = 113;
	}

	std::ofstream result("mapping_files/level_sound_id_map.json");
	result << std::setw(4) << map << std::endl;
	result.close();
	std::cout << "Generated LevelSoundEvent mapping table" << std::endl;
}

static void generate_particle_mapping() {
	auto map = nlohmann::json::object();

	auto list = ParticleTypeMap::getParticleNameTypeList();
	for(auto pair : list) {
		map[pair.first] = (unsigned int) pair.second;
	}

	std::ofstream result("mapping_files/particle_id_map.json");
	result << std::setw(4) << map << std::endl;
	result.close();
	std::cout << "Generated Particle mapping table" << std::endl;
}

extern "C" void modloader_on_server_start(ServerInstance *serverInstance) {
	std::filesystem::create_directory("mapping_files");
	//generate_item_mapping();
	generate_r12_to_current_block_map(serverInstance);
	generate_palette(serverInstance);
	/*generate_biome_mapping(serverInstance);
	generate_level_sound_mapping();
	generate_particle_mapping();
	generate_hardness_table(serverInstance);

	generate_old_to_current_palette_map(serverInstance);*/
}
