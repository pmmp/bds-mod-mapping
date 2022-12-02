#include <minecraft/BinaryStream.h>
#include <minecraft/Block.h>
#include <minecraft/BlockLegacy.h>
#include <minecraft/BlockPalette.h>
#include <minecraft/BlockTypeRegistry.h>
#include <minecraft/CompoundTag.h>
#include <minecraft/Item.h>
#include <minecraft/ItemDescriptor.h>
#include <minecraft/ItemRegistry.h>
#include <minecraft/Level.h>
#include <minecraft/LevelSoundEventMap.h>
#include <minecraft/Memory.h>
#include <minecraft/Minecraft.h>
#include <minecraft/ParticleTypeMap.h>
#include <minecraft/ServerInstance.h>
#include <minecraft/VanillaBlockConversion.h>
#include <minecraft/Biome.h>
#include <minecraft/BiomeRegistry.h>

#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <json.hpp>


void generate_r12_to_current_block_map(ServerInstance *serverInstance) {
	auto palette = serverInstance->getMinecraft()->getLevel()->getBlockPalette();

	std::filesystem::path inputPath{"input_files/r12_block_states.json"};
	if (!std::filesystem::exists(inputPath)) {
		std::cerr << "Input file " << inputPath << " not found, r12_to_current_block_map won't be generated!" << std::endl;
		return;
	}
	std::ifstream input(inputPath);

	auto stream = new BinaryStream();

	auto json = nlohmann::json::object();
	input >> json;

	for (auto &object : json["minecraft"].items()) {
		const auto &name = "minecraft:" + object.key();
		auto blockLegacy = BlockTypeRegistry::lookupByName(name, false).get();
		if (blockLegacy == nullptr){
			std::cerr << "No matching blockstate found for " << name << " (THIS IS A BUG)" << std::endl;
			continue;
		}

		for (auto &it : object.value()) {
			auto state = it.get<unsigned short>();

			if (name == "minecraft:cocoa" && state >= 12) {
				continue;
			}

			auto block = blockLegacy->getStateFromLegacyData(state);
			if (block == nullptr) {
				std::cerr << "No mapped state for " << name << ":" << std::to_string(state) << " (THIS IS A BUG)" << std::endl;
				continue;
			}

			stream->writeUnsignedVarInt(name.length());
			stream->write(name.c_str(), name.length());
			stream->writeUnsignedShort(state);
			stream->writeType(block->tag);
		}
	}

	std::ofstream output("mapping_files/r12_to_current_block_map.bin");
	output << stream->buffer;
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
		if (file.path().extension().string() != ".nbt") {
			continue;
		}
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
	for (unsigned int i = 0; i < numStates; i++) {
		auto state = palette->getBlock(i);
		paletteStream->writeType(state->tag);
	}

	std::ofstream paletteOutput("mapping_files/canonical_block_states.nbt");
	paletteOutput << paletteStream->buffer;
	paletteOutput.close();
	delete paletteStream;
	std::cout << "Generated block palette" << std::endl;
}

static void generate_blockstate_meta_mapping(ServerInstance *serverInstance) {
	auto palette = serverInstance->getMinecraft()->getLevel()->getBlockPalette();
	unsigned int numStates = palette->getNumBlockRuntimeIds();

	auto metaArray = nlohmann::json::array();

	for(auto i = 0; i < numStates; i++){
		auto state = palette->getBlock(i);
		metaArray[i] = state->data;
	}

	std::ofstream out("mapping_files/block_state_meta_map.json");
	out << std::setw(4) << metaArray << std::endl;
	out.close();
	std::cout << "Generated blockstate <-> meta mapping table" << std::endl;
}


static void generate_block_properties_table(ServerInstance *serverInstance) {
	auto palette = serverInstance->getMinecraft()->getLevel()->getBlockPalette();
	unsigned int numStates = palette->getNumBlockRuntimeIds();

	auto table = nlohmann::json::object();

	BlockTypeRegistry::forEachBlock([&table] (const BlockLegacy & blockLegacy)->bool {
		auto name = blockLegacy.getFullName();
		auto data = nlohmann::json::object();
		data["hardness"] = blockLegacy.getDestroySpeed();
		data["mHardness"] = blockLegacy.hardness;
		data["blastResistance"] = blockLegacy.blastResistance;
		data["friction"] = blockLegacy.friction;
		data["flammability"] = blockLegacy.flammability;
		data["flameEncouragement"] = blockLegacy.flameEncouragement;
		data["opacity"] = blockLegacy.opacity;
		data["brightness"] = blockLegacy.brightness;
		table[name] = data;
		return true;
	});

	std::ofstream output("mapping_files/block_properties_table.json");
	output << std::setw(4) << table << std::endl;
	output.close();
	std::cout << "Generated hardness table" << std::endl;
}

void generate_biome_mapping(ServerInstance *server) {
	auto registry = server->getMinecraft()->getLevel()->getBiomeRegistry();

	auto map = nlohmann::json::object();

	registry->forEachBiome([&map] (Biome &biome) {
		auto id = biome.biomeId;
		map[biome.name.str] = id;
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

static std::string add_prefix_if_necessary(std::string input) {
	if (input.rfind("minecraft:", 0) == 0) {
		return input;
	}

	return "minecraft:" + input;
}

static void generate_item_alias_mapping() {
	auto simple = nlohmann::json::object();

	for(auto pair : ItemRegistry::mItemAliasLookupMap) {
		auto prefixed = add_prefix_if_necessary(pair.second.alias.str);
		if (prefixed != pair.first.str) {
			simple[pair.first.str] = prefixed;
		}
	}

	auto complex = nlohmann::json::object();

	for(auto pair : ItemRegistry::mComplexAliasLookupMap) {
		auto metaMap = nlohmann::json::object();

		auto func = pair.second;

		auto zero = func(0).str;
		for(short i = 0; i < 32767; i++){
			auto iStr = func(i).str;
			if (iStr != "" && (i == 0 || iStr != zero)) {
				auto prefixed = add_prefix_if_necessary(iStr);
				if (prefixed != pair.first.str) {
					metaMap[std::to_string(i)] = prefixed;
				}
			}
		}
		complex[pair.first.str] = metaMap;
	}

	auto map = nlohmann::json::object();
	map["simple"] = simple;
	map["complex"] = complex;

	std::ofstream result("mapping_files/r16_to_current_item_map.json");
	result << std::setw(4) << map << std::endl;
	result.close();

	std::cout << "Generated legacy item alias mapping table" << std::endl;
}

static void generate_block_id_to_item_id_map(ServerInstance *serverInstance) {
	auto map = nlohmann::json::object();
	auto palette = serverInstance->getMinecraft()->getLevel()->getBlockPalette();
	unsigned int numStates = palette->getNumBlockRuntimeIds();

	for (unsigned int i = 0; i < numStates; i++) {
		auto state = palette->getBlock(i);
		auto descriptor = new ItemDescriptor(*state);

		const Item *item = descriptor->getItem();
		delete descriptor;
		if (item == nullptr) {
			std::cout << "null item ??? " << state->getLegacyBlock().getFullName() << std::endl;
			continue;
		}
		std::string blockName = state->getLegacyBlock().getFullName();
		std::string itemName = item->getFullItemName();
		map[blockName] = itemName;
	}

	std::ofstream result("mapping_files/block_id_to_item_id_map.json");
	result << std::setw(4) << map << std::endl;
	result.close();

	std::cout << "Generated BlockID to ItemID mapping table" << std::endl;
}

static void generate_command_arg_types_table(ServerInstance *serverInstance) {
	auto map = nlohmann::json::object();

	auto registry = serverInstance->getMinecraft()->getCommands().getRegistry();

	for (int i = 0; i < 1000; i++) { //TODO: we can probably use CommandRegistry::forEachSymbol() for this instead
		int symbol = i | 0x100000;

		if (registry.isValid(symbol)) {
			auto name = registry.symbolToString(symbol);
			auto description = registry.describe(symbol);

			auto object = nlohmann::json::object();
			object["id"] = i;
			object["description"] = description;

			map[name] = object;
		}
	}

	std::ofstream result("mapping_files/command_arg_types.json");
	result << std::setw(4) << map << std::endl;
	result.close();

	std::cout << "Generated command parameter ID mapping table" << std::endl;
}

static void generate_item_tags() {
	auto tags = nlohmann::json::object();
	for (const auto &pair: ItemRegistry::mTagToItemsMap) {
		auto items = nlohmann::json::array();
		for (const auto &item: pair.second) {
			items.push_back(item->getFullItemName());
		}
		tags[pair.first.str] = items;
	}

	std::ofstream result("mapping_files/item_tags.json");
	result << std::setw(4) << tags << std::endl;
	result.close();

	std::cout << "Generated item tags!" << std::endl;
}

extern "C" void modloader_on_server_start(ServerInstance *serverInstance) {
	std::filesystem::create_directory("mapping_files");
	generate_r12_to_current_block_map(serverInstance);
	generate_palette(serverInstance);
	generate_blockstate_meta_mapping(serverInstance);
	generate_biome_mapping(serverInstance);
	generate_level_sound_mapping();
	generate_particle_mapping();
	generate_block_properties_table(serverInstance);

	generate_old_to_current_palette_map(serverInstance);

	generate_item_alias_mapping();
	generate_item_tags();

	generate_block_id_to_item_id_map(serverInstance);
	generate_command_arg_types_table(serverInstance);
}
