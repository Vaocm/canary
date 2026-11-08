/**
 * Canary - A free and open-source MMORPG server emulator
 * Task Board (Bounty Tasks, Weekly Tasks, Hunting Task Shop) - Soul Eater
 */

#include "creatures/players/player.hpp"

#include "creatures/players/components/player_taskboard.hpp"

#include <map>
#include <set>
#include <sstream>

#include "creatures/appearance/mounts/mounts.hpp"
#include "creatures/monsters/monster.hpp"
#include "creatures/monsters/monsters.hpp"
#include "game/game.hpp"
#include "io/iobestiary.hpp"
#include "io/ioprey.hpp"
#include "items/containers/container.hpp"
#include "items/items.hpp"
#include "kv/kv.hpp"
#include "server/network/message/networkmessage.hpp"
#include "server/network/protocol/protocolgame.hpp"
#include "utils/tools.hpp"

namespace TaskBoard {

	namespace {
		const std::vector<std::string> BEGINNER_NAMES = {
			"abyssal calamary",
			"adventurer",
			"amazon",
			"assassin",
			"azure frog",
			"bandit",
			"barbarian brutetamer",
			"barbarian headsplitter",
			"barbarian skullhunter",
			"bear",
			"blood crab",
			"boar",
			"bonelord",
			"calamary",
			"carrion worm",
			"centipede",
			"chakoya toolshaper",
			"chakoya tribewarden",
			"chakoya windcaller",
			"cobra",
			"coral frog",
			"corym charlatan",
			"crab",
			"crazed beggar",
			"crimson frog",
			"crocodile",
			"crypt shambler",
			"cyclops",
			"damaged crystal golem",
			"damaged worker golem",
			"dark apprentice",
			"dark magician",
			"dark monk",
			"deepling worker",
			"deepsea blood crab",
			"dwarf",
			"dwarf guard",
			"dwarf soldier",
			"dworc fleshhunter",
			"dworc venomsniper",
			"dworc voodoomaster",
			"elephant",
			"elf",
			"elf arcanist",
			"elf scout",
			"emerald damselfly",
			"filth toad",
			"fire devil",
			"frost giant",
			"frost giantess",
			"furious troll",
			"gang member",
			"gargoyle",
			"gazer",
			"ghost",
			"ghoul",
			"gladiator",
			"gloom wolf",
			"gnarlhound",
			"goblin assassin",
			"goblin scavenger",
			"gozzler",
			"hunter",
			"hyaena",
			"infernoid blob",
			"infernoid hound",
			"infernoid soul",
			"infernoid spiritual",
			"insect swarm",
			"jellyfish",
			"killer rabbit",
			"kongra",
			"larva",
			"leaf golem",
			"lion",
			"little corym charlatan",
			"lizard executioner",
			"lizard henchman",
			"lizard magician",
			"lizard sentinel",
			"lizard swordmaster",
			"lizard templar",
			"mad scientist",
			"mammoth",
			"marsh stalker",
			"mercury blob",
			"merlkin",
			"minotaur",
			"minotaur archer",
			"minotaur guard",
			"minotaur mage",
			"mole",
			"monk",
			"mummy",
			"nomad",
			"nomad blue",
			"nomad female",
			"novice of the cult",
			"orc",
			"orc rider",
			"orc shaman",
			"orc spearman",
			"orc warrior",
			"orchid frog",
			"panda",
			"pirate cook",
			"pirate ghost",
			"pirate gunner",
			"pirate marauder",
			"pirate navigator",
			"pirate quartermaster",
			"pirate skeleton",
			"poacher",
			"polar bear",
			"quara mantassin scout",
			"rabid wolf",
			"ragged rabid wolf",
			"redeemed soul",
			"rorc",
			"rotworm",
			"salamander",
			"scarab",
			"scorpion",
			"sibang",
			"skeleton",
			"skeleton warrior",
			"slime",
			"slug",
			"smuggler",
			"spit nettle",
			"stalker",
			"stone golem",
			"swamp troll",
			"swampling",
			"tainted soul",
			"tarantula",
			"tarnished spirit",
			"terramite",
			"terrified elephant",
			"terror bird",
			"thornback tortoise",
			"tiger",
			"toad",
			"tortoise",
			"troll champion",
			"undead mine worker",
			"undead prospector",
			"valkyrie",
			"war wolf",
			"white shade",
			"white tiger",
			"wild warrior",
			"witch",
		};
		const std::vector<std::string> ADEPT_NAMES = {
			"abyssal calamary",
			"acid blob",
			"acolyte of the cult",
			"adept of the cult",
			"adventurer",
			"amazon",
			"ancient scarab",
			"animated snowman",
			"arctic faun",
			"askarak demon",
			"askarak lord",
			"askarak prince",
			"assassin",
			"azure frog",
			"baleful bunny",
			"bandit",
			"banshee",
			"barbarian bloodwalker",
			"barbarian brutetamer",
			"barbarian headsplitter",
			"barbarian skullhunter",
			"barkless devotee",
			"barkless fanatic",
			"bear",
			"behemoth",
			"bellicose orger",
			"berserker chicken",
			"betrayed wraith",
			"blood beast",
			"blood crab",
			"blood hand",
			"blood priest",
			"blue djinn",
			"boar",
			"bog raider",
			"bonebeast",
			"bonelord",
			"boogy",
			"braindeath",
			"brimstone bug",
			"broken shaper",
			"calamary",
			"carniphila",
			"carrion worm",
			"centipede",
			"chakoya toolshaper",
			"chakoya tribewarden",
			"chakoya windcaller",
			"clay guardian",
			"clomp",
			"cobra",
			"coral frog",
			"corym charlatan",
			"corym skirmisher",
			"corym vanguard",
			"cow",
			"crab",
			"crawler",
			"crazed beggar",
			"crimson frog",
			"crocodile",
			"crypt defiler",
			"crypt shambler",
			"crystal spider",
			"crystalcrusher",
			"cult believer",
			"cult enforcer",
			"cult scholar",
			"cursed ape",
			"cyclops",
			"cyclops drone",
			"cyclops smith",
			"damaged crystal golem",
			"damaged worker golem",
			"dark apprentice",
			"dark faun",
			"dark magician",
			"dark monk",
			"death blob",
			"death priest",
			"deepling brawler",
			"deepling elite",
			"deepling guard",
			"deepling master librarian",
			"deepling scout",
			"deepling spellsinger",
			"deepling warrior",
			"deepling worker",
			"deepsea blood crab",
			"demon parrot",
			"demon skeleton",
			"destroyer",
			"devourer",
			"diabolic imp",
			"diamond servant replica",
			"doom deer",
			"dragon",
			"dragon hatchling",
			"dragon lord",
			"dragon lord hatchling",
			"dragonling",
			"draken warmaster",
			"drillworm",
			"dwarf",
			"dwarf geomancer",
			"dwarf guard",
			"dwarf henchman",
			"dwarf soldier",
			"dworc fleshhunter",
			"dworc venomsniper",
			"dworc voodoomaster",
			"earth elemental",
			"efreet",
			"elder bonelord",
			"elder forest fury",
			"elder mummy",
			"elephant",
			"elf",
			"elf arcanist",
			"elf scout",
			"emerald damselfly",
			"energy elemental",
			"enfeebled silencer",
			"enlightened of the cult",
			"enraged crystal golem",
			"enslaved dwarf",
			"eternal guardian",
			"evil sheep",
			"evil sheep lord",
			"execowtioner",
			"exotic bat",
			"exotic cave spider",
			"faun",
			"feverish citizen",
			"feversleep",
			"filth toad",
			"fire devil",
			"fire elemental",
			"firestarter",
			"flying book",
			"forest fury",
			"frost dragon",
			"frost dragon hatchling",
			"frost flower asura",
			"frost giant",
			"frost giantess",
			"furious fire elemental",
			"furious troll",
			"gang member",
			"gargoyle",
			"gazer",
			"ghost",
			"ghost wolf",
			"ghoul",
			"ghoulish hyaena",
			"giant spider",
			"gladiator",
			"gloom wolf",
			"glooth anemone",
			"glooth bandit",
			"glooth blob",
			"glooth brigand",
			"glooth golem",
			"gnarlhound",
			"goblin assassin",
			"goblin scavenger",
			"golden servant replica",
			"goldhanded cultist",
			"goldhanded cultist bride",
			"gozzler",
			"grave guard",
			"grave robber",
			"gravedigger",
			"green djinn",
			"gryphon",
			"haunted treeling",
			"hawk hopper",
			"hellspawn",
			"hero",
			"hibernal moth",
			"high voltage elemental",
			"honey elemental",
			"honour guard",
			"hot dog",
			"hunter",
			"hyaena",
			"hydra",
			"ice dragon",
			"ice golem",
			"ice witch",
			"iks aucar",
			"iks chuka",
			"iks churrascan",
			"iks pututu",
			"infernal frog",
			"infernoid blob",
			"infernoid hound",
			"infernoid soul",
			"infernoid spiritual",
			"ink splash",
			"insect swarm",
			"insectoid scout",
			"insectoid worker",
			"instable breach brood",
			"instable sparkion",
			"iron servant replica",
			"jellyfish",
			"jungle moa",
			"killer caiman",
			"killer rabbit",
			"kollos",
			"kongra",
			"lacewing moth",
			"ladybug",
			"lancer beetle",
			"larva",
			"leaf golem",
			"lich",
			"lion",
			"little corym charlatan",
			"lizard chosen",
			"lizard commander",
			"lizard dragon priest",
			"lizard executioner",
			"lizard henchman",
			"lizard high guard",
			"lizard legionnaire",
			"lizard magician",
			"lizard magistratus",
			"lizard sentinel",
			"lizard snakecharmer",
			"lizard swordmaster",
			"lizard templar",
			"lizard zaogun",
			"loricate orger",
			"lost basher",
			"lost exile",
			"lost husher",
			"lost soul",
			"lost thrower",
			"lumbering carnivor",
			"mad scientist",
			"mammoth",
			"manta ray",
			"marid",
			"marsh stalker",
			"massive earth elemental",
			"massive energy elemental",
			"massive fire elemental",
			"massive water elemental",
			"mercury blob",
			"merlkin",
			"metal gargoyle",
			"midnight asura",
			"minotaur",
			"minotaur archer",
			"minotaur cult follower",
			"minotaur cult prophet",
			"minotaur cult zealot",
			"minotaur guard",
			"minotaur hunter",
			"minotaur invader",
			"minotaur mage",
			"misguided bully",
			"misguided thief",
			"mole",
			"monk",
			"mooh'tah warrior",
			"moohtant",
			"mummy",
			"mutated bat",
			"mutated human",
			"mutated rat",
			"mutated tiger",
			"necromancer",
			"nightmare",
			"nightmare scion",
			"nightstalker",
			"noble lion",
			"nomad",
			"nomad blue",
			"nomad female",
			"novice of the cult",
			"nymph",
			"ogre brute",
			"ogre savage",
			"ogre shaman",
			"omnivora",
			"orc",
			"orc berserker",
			"orc cult fanatic",
			"orc cult inquisitor",
			"orc cult minion",
			"orc cult priest",
			"orc cultist",
			"orc leader",
			"orc marauder",
			"orc rider",
			"orc shaman",
			"orc spearman",
			"orc warlord",
			"orc warrior",
			"orchid frog",
			"orclops doomhauler",
			"orclops ravager",
			"orger",
			"panda",
			"parder",
			"percht",
			"pirat bombardier",
			"pirat cutthroat",
			"pirat mate",
			"pirat scoundrel",
			"pirate buccaneer",
			"pirate cook",
			"pirate corsair",
			"pirate cutthroat",
			"pirate ghost",
			"pirate gunner",
			"pirate marauder",
			"pirate navigator",
			"pirate quartermaster",
			"pirate skeleton",
			"pixie",
			"plaguesmith",
			"poacher",
			"polar bear",
			"pooka",
			"priestess",
			"putrid mummy",
			"quara constrictor",
			"quara constrictor scout",
			"quara hydromancer",
			"quara hydromancer scout",
			"quara mantassin",
			"quara mantassin scout",
			"quara pincher",
			"quara pincher scout",
			"quara predator",
			"quara predator scout",
			"rabid wolf",
			"ragged rabid wolf",
			"ravenous lava lurker",
			"redeemed soul",
			"renegade knight",
			"renegade quara constrictor",
			"renegade quara hydromancer",
			"renegade quara mantassin",
			"renegade quara pincher",
			"renegade quara predator",
			"roaring lion",
			"roast pork",
			"rorc",
			"rot elemental",
			"rotworm",
			"rustheap golem",
			"sacred spider",
			"salamander",
			"sandstone scorpion",
			"scarab",
			"schiach",
			"scorpion",
			"sea captain",
			"sea serpent",
			"serpent spawn",
			"shaburak demon",
			"shaburak lord",
			"shaburak prince",
			"shadow pupil",
			"shaper matriarch",
			"shark",
			"sibang",
			"silencer",
			"skeleton",
			"skeleton warrior",
			"slime",
			"slug",
			"smuggler",
			"souleater",
			"spectre",
			"spidris",
			"spidris elite",
			"spit nettle",
			"spitter",
			"squidgy slime",
			"stabilizing dread intruder",
			"stabilizing reality reaver",
			"stalker",
			"stampor",
			"starving wolf",
			"stone golem",
			"stone rhino",
			"stonerefiner",
			"swamp troll",
			"swampling",
			"swan maiden",
			"swarmer",
			"tainted soul",
			"tarantula",
			"tarnished spirit",
			"terramite",
			"terrified elephant",
			"terror bird",
			"thornback tortoise",
			"tiger",
			"toad",
			"tomb servant",
			"tortoise",
			"troll champion",
			"troll legionnaire",
			"twisted pooka",
			"twisted shaper",
			"undead gladiator",
			"undead mine worker",
			"undead prospector",
			"valkyrie",
			"vampire",
			"vampire bride",
			"vampire pig",
			"vampire viscount",
			"vicious squire",
			"vile grandmaster",
			"wailing widow",
			"walker",
			"war golem",
			"war wolf",
			"warlock",
			"waspoid",
			"water elemental",
			"weakened frazzlemaw",
			"werebadger",
			"werebear",
			"wereboar",
			"werefox",
			"werehyaena",
			"werehyaena shaman",
			"werewolf",
			"white shade",
			"white tiger",
			"wiggler",
			"wild warrior",
			"wilting leaf golem",
			"witch",
			"worker golem",
			"worm priestess",
			"wyrm",
			"wyvern",
			"yielothax",
			"young sea serpent",
			"zombie",
		};
		const std::vector<std::string> EXPERT_NAMES = {
			"acid blob",
			"acolyte of the cult",
			"adept of the cult",
			"adult goanna",
			"afflicted strider",
			"ancient scarab",
			"angry sugar fairy",
			"animated feather",
			"animated snowman",
			"arachnophobica",
			"arctic faun",
			"armadile",
			"askarak demon",
			"askarak lord",
			"askarak prince",
			"baleful bunny",
			"banshee",
			"barbarian bloodwalker",
			"barkless devotee",
			"barkless fanatic",
			"bashmu",
			"behemoth",
			"bellicose orger",
			"berserker chicken",
			"betrayed wraith",
			"biting book",
			"black sphinx acolyte",
			"blemished spawn",
			"blightwalker",
			"blood beast",
			"blood hand",
			"blood priest",
			"blue djinn",
			"bluebeak",
			"boar man",
			"bog raider",
			"bonebeast",
			"boogy",
			"brain squid",
			"braindeath",
			"bramble wyrmling",
			"breach brood",
			"brimstone bug",
			"brinebrute inferniarch",
			"broken shaper",
			"broodrider inferniarch",
			"bulltaur alchemist",
			"bulltaur brute",
			"bulltaur forgepriest",
			"burning book",
			"burning gladiator",
			"burster spectre",
			"candy floss elemental",
			"candy horror",
			"carniphila",
			"carnivostrich",
			"cave chimera",
			"cave devourer",
			"chasm spawn",
			"choking fear",
			"cinder wyrmling",
			"clay guardian",
			"cliff strider",
			"clomp",
			"cobra assassin",
			"cobra scout",
			"cobra vizier",
			"corym skirmisher",
			"corym vanguard",
			"cow",
			"crape man",
			"crawler",
			"crazed summer rearguard",
			"crazed summer vanguard",
			"crazed winter rearguard",
			"crazed winter vanguard",
			"crusader",
			"crypt mage",
			"crypt warden",
			"crypt warrior",
			"crystal spider",
			"crystalcrusher",
			"cult believer",
			"cult enforcer",
			"cult scholar",
			"cunning werepanther",
			"cursed ape",
			"cursed book",
			"cursed prospector",
			"cyclops drone",
			"cyclops smith",
			"cyclursus",
			"dark carnisylvan",
			"dark faun",
			"dark torturer",
			"dawnfire asura",
			"death blob",
			"death priest",
			"deathling scout",
			"deathling spellsinger",
			"deepling brawler",
			"deepling elite",
			"deepling guard",
			"deepling master librarian",
			"deepling scout",
			"deepling spellsinger",
			"deepling tyrant",
			"deepling warrior",
			"deepworm",
			"defiler",
			"demon",
			"demon outcast",
			"demon parrot",
			"demon skeleton",
			"destroyer",
			"devourer",
			"diabolic imp",
			"diamond servant replica",
			"diremaw",
			"doom deer",
			"dragolisk",
			"dragon",
			"dragon hatchling",
			"dragon lord",
			"dragon lord hatchling",
			"dragonling",
			"draken abomination",
			"draken elite",
			"draken spellweaver",
			"draken warmaster",
			"dread intruder",
			"drillworm",
			"dwarf geomancer",
			"dwarf henchman",
			"dworc shadowstalker",
			"earth elemental",
			"efreet",
			"elder bonelord",
			"elder forest fury",
			"elder mummy",
			"elder wyrm",
			"energetic book",
			"energuardian of tales",
			"energy elemental",
			"enfeebled silencer",
			"enlightened of the cult",
			"enraged crystal golem",
			"enslaved dwarf",
			"eternal guardian",
			"evil prospector",
			"evil sheep",
			"evil sheep lord",
			"execowtioner",
			"exotic bat",
			"exotic cave spider",
			"eyeless devourer",
			"falcon knight",
			"falcon paladin",
			"faun",
			"feral sphinx",
			"feral werecrocodile",
			"feversleep",
			"fire elemental",
			"flimsy lost soul",
			"floating savant",
			"flying book",
			"foam stalker",
			"forest fury",
			"frazzlemaw",
			"freakish lost soul",
			"frost dragon",
			"frost dragon hatchling",
			"frost flower asura",
			"furious fire elemental",
			"fury",
			"gazer spectre",
			"ghastly dragon",
			"ghoulish hyaena",
			"giant spider",
			"girtablilu warrior",
			"gloom maw",
			"glooth anemone",
			"glooth bandit",
			"glooth blob",
			"glooth brigand",
			"glooth golem",
			"goggle cake",
			"golden servant replica",
			"goldhanded cultist",
			"goldhanded cultist bride",
			"gorger inferniarch",
			"grave guard",
			"gravedigger",
			"green djinn",
			"grim reaper",
			"grimeleech",
			"gryphon",
			"guardian of tales",
			"guzzlemaw",
			"hand of cursed fate",
			"harpy",
			"haunted dragon",
			"haunted treeling",
			"hawk hopper",
			"headwalker",
			"hellfire fighter",
			"hellflayer",
			"hellhound",
			"hellhunter inferniarch",
			"hellspawn",
			"hero",
			"hibernal moth",
			"hideous fungus",
			"high voltage elemental",
			"hive overseer",
			"honey elemental",
			"hot dog",
			"hulking carnisylvan",
			"humongous fungus",
			"hydra",
			"ice dragon",
			"ice golem",
			"ice witch",
			"icecold book",
			"iceplume strider",
			"iks aucar",
			"iks chuka",
			"iks churrascan",
			"iks pututu",
			"iks yapunac",
			"infected weeper",
			"infernal frog",
			"infernalist",
			"ink blob",
			"ink splash",
			"insane siren",
			"insectoid worker",
			"instable breach brood",
			"instable sparkion",
			"iron servant replica",
			"ironblight",
			"jaracal",
			"juggernaut",
			"jungle moa",
			"juvenile bashmu",
			"killer caiman",
			"knowledge elemental",
			"kollos",
			"lacewing moth",
			"lamassu",
			"lancer beetle",
			"lava golem",
			"lava lurker",
			"lavafungus",
			"lavaworm",
			"lich",
			"liodile",
			"lion hydra",
			"lizard chosen",
			"lizard dragon priest",
			"lizard high guard",
			"lizard legionnaire",
			"lizard magistratus",
			"lizard noble",
			"lizard snakecharmer",
			"lizard zaogun",
			"loricate orger",
			"lost basher",
			"lost berserker",
			"lost exile",
			"lost husher",
			"lost soul",
			"lost thrower",
			"lumbering carnivor",
			"magma crawler",
			"makara",
			"manta ray",
			"manticore",
			"marid",
			"massive earth elemental",
			"massive energy elemental",
			"massive fire elemental",
			"massive water elemental",
			"mean lost soul",
			"medusa",
			"mega dragon",
			"menacing carnivor",
			"metal gargoyle",
			"midnight asura",
			"minotaur amazon",
			"minotaur cult follower",
			"minotaur cult prophet",
			"minotaur cult zealot",
			"minotaur hunter",
			"minotaur invader",
			"misguided bully",
			"misguided thief",
			"mitmah scout",
			"mitmah seer",
			"mooh'tah warrior",
			"moohtant",
			"moonspawn blightspitter",
			"moonspawn oozecrown",
			"moonstone excavator",
			"moonstone overseer",
			"mutated bat",
			"mutated human",
			"mutated rat",
			"mutated tiger",
			"naga archer",
			"naga warrior",
			"necromancer",
			"nibblemaw",
			"night harpy",
			"nightmare",
			"nightmare scion",
			"nightstalker",
			"noble lion",
			"norcferatu heartless",
			"norcferatu nightweaver",
			"nymph",
			"ogre brute",
			"ogre rowdy",
			"ogre ruffian",
			"ogre sage",
			"ogre savage",
			"ogre shaman",
			"omnivora",
			"orc berserker",
			"orc cult fanatic",
			"orc cult inquisitor",
			"orc cult minion",
			"orc cult priest",
			"orc cultist",
			"orc leader",
			"orc marauder",
			"orc warlord",
			"orclops bloodbreaker",
			"orclops doomhauler",
			"orclops ravager",
			"orewalker",
			"orger",
			"parder",
			"percht",
			"phantasm",
			"pirat bombardier",
			"pirat cutthroat",
			"pirat mate",
			"pirat scoundrel",
			"pirate buccaneer",
			"pirate corsair",
			"pirate cutthroat",
			"pixie",
			"plaguesmith",
			"poisonous carnisylvan",
			"pooka",
			"priestess",
			"priestess of the wild sun",
			"putrid mummy",
			"quara constrictor",
			"quara constrictor scout",
			"quara hydromancer",
			"quara hydromancer scout",
			"quara looter",
			"quara mantassin",
			"quara pincher",
			"quara pincher scout",
			"quara plunderer",
			"quara predator",
			"quara predator scout",
			"quara raider",
			"rage squid",
			"raubritter chastener",
			"raubritter marksman",
			"raubritter skirmisher",
			"ravenous lava lurker",
			"reality reaver",
			"renegade knight",
			"renegade quara constrictor",
			"renegade quara hydromancer",
			"renegade quara mantassin",
			"renegade quara pincher",
			"renegade quara predator",
			"retching horror",
			"rhindeer",
			"ripper spectre",
			"roaming dread",
			"roaring lion",
			"roast pork",
			"rootthing amber shaper",
			"rootthing bug tracker",
			"rootthing nutshell",
			"rot elemental",
			"rustheap golem",
			"sacred spider",
			"sandstone scorpion",
			"schiach",
			"sea serpent",
			"seacrest serpent",
			"serpent spawn",
			"shaburak demon",
			"shaburak lord",
			"shaburak prince",
			"shadow pupil",
			"shaper matriarch",
			"shark",
			"shell drake",
			"shock head",
			"sight of surrender",
			"silencer",
			"silverfrost sentinel",
			"sineater inferniarch",
			"skeleton elite warrior",
			"son of verminor",
			"soul-broken harbinger",
			"souleater",
			"sparkion",
			"spectre",
			"spellreaper inferniarch",
			"sphinx",
			"spidris",
			"spidris elite",
			"spiky carnivor",
			"spitter",
			"squid warden",
			"stabilizing dread intruder",
			"stabilizing reality reaver",
			"stampor",
			"stone devourer",
			"stone rhino",
			"stonerefiner",
			"streaked devourer",
			"swan maiden",
			"swarmer",
			"terrorsleep",
			"thanatursus",
			"tomb servant",
			"tremendous tyrant",
			"troll legionnaire",
			"true dawnfire asura",
			"true feverbloom asura",
			"true frost flower asura",
			"true midnight asura",
			"tunnel tyrant",
			"twisted pooka",
			"twisted shaper",
			"two-headed turtle",
			"undead dragon",
			"undead elite gladiator",
			"undead gladiator",
			"usurper archer",
			"usurper knight",
			"usurper warlock",
			"vampire",
			"vampire bride",
			"vampire pig",
			"vampire viscount",
			"varg",
			"varnished diremaw",
			"venerable girtablilu",
			"vexclaw",
			"vicious squire",
			"vile grandmaster",
			"vulcongra",
			"wailing widow",
			"walker",
			"war golem",
			"wardragon",
			"warlock",
			"waspoid",
			"water elemental",
			"weakened frazzlemaw",
			"weeper",
			"werebadger",
			"werebear",
			"wereboar",
			"werecrocodile",
			"werefox",
			"werehyaena",
			"werehyaena shaman",
			"werelion",
			"werelioness",
			"werepanther",
			"weretiger",
			"werewolf",
			"white lion",
			"white weretiger",
			"wiggler",
			"wilting leaf golem",
			"winged jaracal",
			"worker golem",
			"worm priestess",
			"wyrm",
			"wyvern",
			"yielothax",
			"young goanna",
			"young sea serpent",
			"zombie",
		};
		const std::vector<std::string> MASTER_NAMES = {
			"adult goanna",
			"afflicted strider",
			"angry sugar fairy",
			"animated feather",
			"arachnophobica",
			"armadile",
			"bashmu",
			"biting book",
			"black sphinx acolyte",
			"blemished spawn",
			"blightwalker",
			"bloated man-maggot",
			"bluebeak",
			"boar man",
			"bony sea devil",
			"brachiodemon",
			"brain squid",
			"bramble wyrmling",
			"branchy crawler",
			"breach brood",
			"brinebrute inferniarch",
			"broodrider inferniarch",
			"bulltaur alchemist",
			"bulltaur brute",
			"bulltaur forgepriest",
			"burning book",
			"burning gladiator",
			"burster spectre",
			"candy floss elemental",
			"candy horror",
			"capricious phantom",
			"carnivostrich",
			"cave chimera",
			"cave devourer",
			"chasm spawn",
			"choking fear",
			"cinder wyrmling",
			"cliff strider",
			"cloak of terror",
			"cobra assassin",
			"cobra scout",
			"cobra vizier",
			"converter",
			"courage leech",
			"crape man",
			"crazed summer rearguard",
			"crazed summer vanguard",
			"crazed winter rearguard",
			"crazed winter vanguard",
			"creepy crawler",
			"crusader",
			"crypt construct",
			"crypt fiend",
			"crypt mage",
			"crypt warden",
			"crypt warrior",
			"cunning werepanther",
			"cursed book",
			"cursed prospector",
			"cyclursus",
			"dark carnisylvan",
			"dark torturer",
			"darklight construct",
			"darklight emitter",
			"darklight matter",
			"darklight source",
			"darklight striker",
			"dawnfire asura",
			"deathling scout",
			"deathling spellsinger",
			"deepling tyrant",
			"deepworm",
			"defiler",
			"demon",
			"demon outcast",
			"devoted radiant acolyte",
			"devoted radiant inquisitor",
			"devoted radiant paragon",
			"devoted radiant templar",
			"devoted radiant warden",
			"devoted radiant zealot",
			"diremaw",
			"distorted phantom",
			"dragolisk",
			"draken abomination",
			"draken elite",
			"draken spellweaver",
			"dread intruder",
			"druid's apparition",
			"dworc shadowstalker",
			"elder wyrm",
			"emerald tortoise",
			"energetic book",
			"energuardian of tales",
			"evil prospector",
			"eyeless devourer",
			"falcon knight",
			"falcon paladin",
			"feral sphinx",
			"feral werecrocodile",
			"flimsy lost soul",
			"floating savant",
			"foam stalker",
			"frazzlemaw",
			"freakish lost soul",
			"fury",
			"gazer spectre",
			"ghastly dragon",
			"girtablilu warrior",
			"gloom maw",
			"goggle cake",
			"gore horn",
			"gorerilla",
			"gorger inferniarch",
			"grim reaper",
			"grimeleech",
			"guardian of tales",
			"guzzlemaw",
			"hand of cursed fate",
			"harpy",
			"haunted dragon",
			"haunted hunter",
			"headpecker",
			"headwalker",
			"hellfire fighter",
			"hellflayer",
			"hellhound",
			"hellhunter inferniarch",
			"hideous fungus",
			"hive overseer",
			"hulking carnisylvan",
			"hulking prehemoth",
			"humongous fungus",
			"icecold book",
			"iceplume strider",
			"iks yapunac",
			"infected weeper",
			"infernal demon",
			"infernal phantom",
			"infernalist",
			"ink blob",
			"insane siren",
			"ironblight",
			"jaracal",
			"juggernaut",
			"juvenile bashmu",
			"knight's apparition",
			"knowledge elemental",
			"lamassu",
			"lava golem",
			"lava lurker",
			"lavafungus",
			"lavaworm",
			"liodile",
			"lion hydra",
			"lizard noble",
			"lost berserker",
			"magma crawler",
			"makara",
			"manticore",
			"mantosaurus",
			"many faces",
			"mean lost soul",
			"meandering mushroom",
			"medusa",
			"mega dragon",
			"menacing carnivor",
			"mercurial menace",
			"minotaur amazon",
			"mitmah scout",
			"mitmah seer",
			"monk's apparition",
			"moonspawn blightspitter",
			"moonspawn oozecrown",
			"moonstone excavator",
			"moonstone overseer",
			"mould phantom",
			"mycobiontic beetle",
			"naga archer",
			"naga warrior",
			"nibblemaw",
			"night harpy",
			"nighthunter",
			"norcferatu heartless",
			"norcferatu nightweaver",
			"noxious ripptor",
			"ogre rowdy",
			"ogre ruffian",
			"ogre sage",
			"oozing carcass",
			"oozing corpus",
			"orclops bloodbreaker",
			"orewalker",
			"paladin's apparition",
			"phantasm",
			"poisonous carnisylvan",
			"priestess of the wild sun",
			"quara looter",
			"quara plunderer",
			"quara raider",
			"radiant acolyte",
			"radiant inquisitor",
			"radiant paragon",
			"radiant templar",
			"radiant warden",
			"radiant zealot",
			"rage squid",
			"raubritter chastener",
			"raubritter marksman",
			"raubritter skirmisher",
			"reality reaver",
			"retching horror",
			"rhindeer",
			"ripper spectre",
			"roaming dread",
			"rootthing amber shaper",
			"rootthing bug tracker",
			"rootthing nutshell",
			"rotten golem",
			"rotten man-maggot",
			"sabretooth (creature)",
			"seacrest serpent",
			"shell drake",
			"shock head",
			"shrieking cry-stal",
			"sight of surrender",
			"silverfrost sentinel",
			"sineater inferniarch",
			"skeleton elite warrior",
			"son of verminor",
			"sopping carcass",
			"sopping corpus",
			"sorcerer's apparition",
			"soul-broken harbinger",
			"sparkion",
			"spellreaper inferniarch",
			"sphinx",
			"spiky carnivor",
			"squid warden",
			"stalking stalk",
			"stone devourer",
			"streaked devourer",
			"sulphider",
			"sulphur spouter",
			"terrorsleep",
			"thanatursus",
			"tremendous tyrant",
			"true dawnfire asura",
			"true feverbloom asura",
			"true frost flower asura",
			"true midnight asura",
			"tunnel tyrant",
			"turbulent elemental",
			"two-headed turtle",
			"undead dragon",
			"undead elite gladiator",
			"undertaker",
			"usurper archer",
			"usurper knight",
			"usurper warlock",
			"varg",
			"varnished diremaw",
			"venerable girtablilu",
			"vexclaw",
			"vibrant phantom",
			"vulcongra",
			"walking dread",
			"walking pillar",
			"wandering pillar",
			"wardragon",
			"weeper",
			"werecrocodile",
			"werelion",
			"werelioness",
			"werepanther",
			"weretiger",
			"white lion",
			"white weretiger",
			"winged jaracal",
			"young goanna",
		};

		std::string toLower(std::string s) {
			std::ranges::transform(s, s.begin(), [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
			return s;
		}

		bool isBlockedLootName(const std::string &name) {
			static const std::vector<std::string> blocked = { "coin", "nugget", "gem", "potion", "amulet", "ring", "scroll", "key", "bag", "backpack", "rune", "arrow", "bolt", "dollar", "tibia", "mead", "bottle", "vial", "flask", "token" };
			return std::ranges::any_of(blocked, [&name](const std::string &b) { return name.find(b) != std::string::npos; });
		}
	}

	uint8_t getTalismanMaxLevel(uint8_t path) {
		return path == 3 ? 180 : 166;
	}

	uint8_t getTalismanStartLevel(uint8_t path) {
		return path == 3 ? 5 : 1;
	}

	// Mirrors the table used by the client so that what is displayed is what is applied.
	uint32_t getTalismanBonusHundredths(uint8_t level, uint8_t path) {
		if (level == 0) {
			return 0;
		}
		if (path <= 2) {
			if (level <= 26) {
				return 250 + (static_cast<uint32_t>(level) - 1) * 50;
			}
			return std::min<uint32_t>(1500 + (static_cast<uint32_t>(level) - 26) * 25, 5000);
		}
		if (level <= 20) {
			return static_cast<uint32_t>(level) * 100;
		}
		return std::min<uint32_t>(2000 + (static_cast<uint32_t>(level) - 20) * 50, 10000);
	}

	// Experience of a delivery task / weekly kill task (Tibia wiki formula)
	uint64_t getLevelExperienceReward(uint32_t level) {
		const uint64_t l = level;
		if (level <= 82) {
			return 25 * l * l - 75 * l + 100;
		}
		if (level < 1000) {
			return static_cast<uint64_t>(std::llround(1994.008 * static_cast<double>(l)));
		}
		return 2 * l * l - 6 * l + 8;
	}

	Catalog &Catalog::getInstance() {
		static Catalog instance;
		return instance;
	}

	const std::vector<uint16_t> &Catalog::getPool(uint8_t difficulty) {
		ensureLoaded();
		return m_pools[std::min<uint8_t>(difficulty, DIFFICULTY_COUNT - 1)];
	}

	const std::vector<Catalog::DeliveryCandidate> &Catalog::getDeliveryPool(uint8_t difficulty) {
		ensureLoaded();
		return m_delivery[std::min<uint8_t>(difficulty, DIFFICULTY_COUNT - 1)];
	}

	const std::vector<ShopOffer> &Catalog::getShop() {
		ensureLoaded();
		return m_shop;
	}

	bool Catalog::isInAnyPool(uint16_t raceId) {
		ensureLoaded();
		return std::ranges::any_of(m_pools, [raceId](const std::vector<uint16_t> &pool) {
			return std::ranges::find(pool, raceId) != pool.end();
		});
	}

	uint32_t Catalog::getMonsterExperience(uint16_t raceId) const {
		const auto &mType = g_monsters().getMonsterTypeByRaceId(raceId);
		if (!mType) {
			return 0;
		}
		return static_cast<uint32_t>(std::min<uint64_t>(mType->info.experience, std::numeric_limits<uint32_t>::max()));
	}

	void Catalog::ensureLoaded() {
		if (m_loaded) {
			return;
		}
		m_loaded = true;

		// ---- Creature pools: exact Tibia wiki lists per difficulty ----
		const std::array<std::set<std::string>, DIFFICULTY_COUNT> tierNames = { {
			std::set<std::string>(BEGINNER_NAMES.begin(), BEGINNER_NAMES.end()),
			std::set<std::string>(ADEPT_NAMES.begin(), ADEPT_NAMES.end()),
			std::set<std::string>(EXPERT_NAMES.begin(), EXPERT_NAMES.end()),
			std::set<std::string>(MASTER_NAMES.begin(), MASTER_NAMES.end()),
		} };
		std::set<uint16_t> seen;
		for (const auto &[key, mType] : g_monsters().monsters) {
			if (!mType || mType->info.raceid == 0 || mType->isBoss()) {
				continue;
			}
			const auto raceId = mType->info.raceid;
			if (!seen.insert(raceId).second) {
				continue;
			}
			const auto lowerName = toLower(mType->name);
			for (uint8_t d = 0; d < DIFFICULTY_COUNT; ++d) {
				if (tierNames[d].contains(lowerName)) {
					m_pools[d].push_back(raceId);
				}
			}
		}
		for (auto &pool : m_pools) {
			std::ranges::sort(pool);
		}

		// ---- Delivery item candidates (creature products dropped by the pool creatures) ----
		for (uint8_t d = 0; d < DIFFICULTY_COUNT; ++d) {
			std::map<uint16_t, DeliveryCandidate> candidates;
			for (const auto raceId : m_pools[d]) {
				const auto &mType = g_monsters().getMonsterTypeByRaceId(raceId);
				if (!mType) {
					continue;
				}
				for (const auto &loot : mType->info.lootItems) {
					if (loot.id == 0 || loot.chance < 2000 || !loot.childLoot.empty()) {
						continue;
					}
					const auto &it = Item::items[loot.id];
					if (it.id == 0 || !it.isPickupable() || it.weaponType != WEAPON_NONE || it.isContainer() || it.isRune() || it.isFluidContainer() || it.isCorpse || it.isWrappable() || it.isMagicField()) {
						continue;
					}
					if (it.slotPosition & (SLOTP_HEAD | SLOTP_NECKLACE | SLOTP_BACKPACK | SLOTP_ARMOR | SLOTP_LEGS | SLOTP_FEET | SLOTP_RING | SLOTP_AMMO | SLOTP_TWO_HAND)) {
						continue;
					}
					if (it.armor != 0 || it.defense != 0 || it.attack != 0 || it.name.empty() || isBlockedLootName(toLower(it.name))) {
						continue;
					}
					auto &cand = candidates[loot.id];
					cand.itemId = loot.id;
					cand.chance = std::max<uint32_t>(cand.chance, loot.chance);
					cand.avgCount = std::max<uint32_t>(1, (loot.countmin + loot.countmax) / 2);
				}
			}
			for (const auto &[id, cand] : candidates) {
				m_delivery[d].push_back(cand);
			}
		}

		// ---- Hunting Task Shop (prices: Tibia wiki / screenshots) ----
		auto addItem = [this](const std::string &title, const std::string &desc, uint32_t price, uint16_t itemId) {
			ShopOffer o;
			o.type = SHOP_ITEM;
			o.title = title;
			o.description = desc;
			o.price = price;
			o.itemId = itemId;
			m_shop.push_back(o);
		};
		auto addOutfit = [this](const std::string &title, const std::string &desc, uint32_t price, uint16_t male, uint16_t female, uint8_t addons) {
			ShopOffer o;
			o.type = SHOP_OUTFIT;
			o.title = title;
			o.description = desc;
			o.price = price;
			o.lookTypeMale = male;
			o.lookTypeFemale = female;
			o.addons = addons;
			m_shop.push_back(o);
		};
		auto addMount = [this](const std::string &title, const std::string &desc, uint32_t price, uint16_t mountId) {
			ShopOffer o;
			o.type = SHOP_MOUNT;
			o.title = title;
			o.description = desc;
			o.price = price;
			o.mountId = mountId;
			m_shop.push_back(o);
		};

		addOutfit("Falconer (Base Outfit)", "Show off your hunting skills wearing this fashionable outfit.", 100000, 1282, 1283, 0);
		addOutfit("Falconer (Addon 1)", "Boar-ed running around without a helmet?", 35000, 1282, 1283, 1);
		addOutfit("Falconer (Addon 2)", "Pro: Fal Con: None", 35000, 1282, 1283, 2);
		addMount("Antelope (Mount)", "A true hunters loyal companion.", 145000, 163);
		addItem("Falcon Pet", "A pet for decorative purposes.", 135000, 36750);
		addItem("Cerberus Champion Puppy", "A pet for decorative purposes.", 75000, 31464);
		const std::string trophyDesc = "A trophy from Walter Jaeger, commemorating your mastery in the wilds.";
		addItem("Hellflayer Trophy", trophyDesc, 80000, 32753);
		addItem("Gold Hunter Trophy", trophyDesc, 80000, 32756);
		addItem("Brachiodemon Trophy", trophyDesc, 80000, 36748);
		addItem("Many Faces Trophy", trophyDesc, 50000, 36749);
		addItem("Sea Serpent Trophy", trophyDesc, 15000, 32752);
		addItem("Silver Hunter Trophy", trophyDesc, 15000, 32755);
		addItem("Bronze Hunter Trophy", trophyDesc, 3000, 32754);
		addItem("Gozzler Trophy", trophyDesc, 3000, 32751);
		addItem("Demon Doll", "A doll for skilled hunters, honouring your proven prowess in battle.", 37500, 32918);
		addItem("Vexclaw Doll", "A doll for skilled hunters, honouring your proven prowess in battle.", 37500, 32943);
		addItem("Bone Bed", "A bed carved from the bones of your fallen foes.", 35000, 32799);
		ShopOffer promo;
		promo.type = SHOP_PROMOTION;
		promo.title = "Bonus Promotion Point";
		m_shop.push_back(promo);
	}

}

// ---------------------------------------------------------------------------------------
// Time helpers
// ---------------------------------------------------------------------------------------
namespace {
	std::tm toLocalTm(std::time_t t) {
		std::tm out {};
#ifdef _WIN32
		localtime_s(&out, &t);
#else
		localtime_r(&t, &out);
#endif
		return out;
	}

	// Next occurrence of hour:00 local time strictly after `now`
	int64_t nextDailyReset(int64_t now) {
		auto lt = toLocalTm(static_cast<std::time_t>(now));
		lt.tm_hour = 10;
		lt.tm_min = 0;
		lt.tm_sec = 0;
		lt.tm_isdst = -1;
		auto candidate = static_cast<int64_t>(std::mktime(&lt));
		if (candidate <= now) {
			lt.tm_mday += 1;
			lt.tm_isdst = -1;
			candidate = static_cast<int64_t>(std::mktime(&lt));
		}
		return candidate;
	}

	// Next Monday 10:00 local time strictly after `now` (weekly tasks reset on Mondays)
	int64_t nextWeeklyReset(int64_t now) {
		auto lt = toLocalTm(static_cast<std::time_t>(now));
		const int daysAhead = (1 - lt.tm_wday + 7) % 7;
		lt.tm_mday += daysAhead;
		lt.tm_hour = 10;
		lt.tm_min = 0;
		lt.tm_sec = 0;
		lt.tm_isdst = -1;
		auto candidate = static_cast<int64_t>(std::mktime(&lt));
		if (candidate <= now) {
			lt.tm_mday += 7;
			lt.tm_isdst = -1;
			candidate = static_cast<int64_t>(std::mktime(&lt));
		}
		return candidate;
	}

	int64_t nowSeconds() {
		return static_cast<int64_t>(std::time(nullptr));
	}

	uint32_t clampU32(uint64_t v) {
		return static_cast<uint32_t>(std::min<uint64_t>(v, std::numeric_limits<uint32_t>::max()));
	}
}

// ---------------------------------------------------------------------------------------
// PlayerTaskBoard
// ---------------------------------------------------------------------------------------
PlayerTaskBoard::PlayerTaskBoard(Player &player) :
	m_player(player) { }

void PlayerTaskBoard::load() {
	m_state = State {};
	m_state.slots[0].unlocked = true;
	const auto value = m_player.kv()->get("taskboard");
	if (value) {
		const auto data = value->get<std::string>();
		if (!data.empty() && !deserialize(data)) {
			g_logger().warn("[PlayerTaskBoard::load] invalid task board data for player {}, resetting", m_player.getName());
			m_state = State {};
			m_state.slots[0].unlocked = true;
		}
	}
	m_loaded = true;
}

void PlayerTaskBoard::ensureLoaded() {
	if (!m_loaded) {
		load();
	}
	checkWeeklyRollover();
}

void PlayerTaskBoard::save() {
	if (!m_loaded) {
		return;
	}
	m_player.kv()->set("taskboard", ValueWrapper(serialize()));
}

std::string PlayerTaskBoard::serialize() const {
	std::ostringstream out;
	const auto &s = m_state;
	out << "TB1 " << static_cast<int>(s.difficulty) << ' ' << (s.offersGenerated ? 1 : 0) << ' ' << static_cast<int>(s.activeIndex) << ' ' << static_cast<int>(s.rerollTokens) << ' ' << s.bountyPoints << ' ' << s.nextDailyClaim << ' ';
	for (const auto lvl : s.talisman) {
		out << static_cast<int>(lvl) << ' ';
	}
	for (const auto &slot : s.slots) {
		out << (slot.unlocked ? 1 : 0) << ' ' << slot.preferred << ' ' << slot.unwanted << ' ';
	}
	for (const auto &o : s.offers) {
		out << o.raceId << ' ' << o.total << ' ' << o.current << ' ' << o.xp << ' ' << static_cast<int>(o.points) << ' ' << static_cast<int>(o.rarity) << ' ' << static_cast<int>(o.state) << ' ';
	}
	out << (s.weeklyStarted ? 1 : 0) << ' ' << (s.weeklyFinished ? 1 : 0) << ' ' << static_cast<int>(s.weeklyDifficulty) << ' ' << s.weeklyReset << ' ' << s.anyTotal << ' ' << s.anyCurrent << ' ' << (s.anyRewarded ? 1 : 0) << ' ' << s.lastPoints << ' ' << s.lastSeals << ' ';
	out << s.weeklyKills.size() << ' ';
	for (const auto &k : s.weeklyKills) {
		out << k.raceId << ' ' << k.total << ' ' << k.current << ' ' << (k.rewarded ? 1 : 0) << ' ';
	}
	out << s.weeklyDeliveries.size() << ' ';
	for (const auto &d : s.weeklyDeliveries) {
		out << d.itemId << ' ' << d.total << ' ' << (d.claimed ? 1 : 0) << ' ';
	}
	out << s.soulseals << ' ' << static_cast<int>(s.promotionBought) << ' ' << (s.weeklyExpansion ? 1 : 0) << ' ' << s.shopBought.size() << ' ';
	for (const auto b : s.shopBought) {
		out << static_cast<int>(b) << ' ';
	}
	out << s.pointsSpent << ' ' << s.completedTasks[0] << ' ' << s.completedTasks[1] << ' ' << s.completedTasks[2] << ' ';
	return out.str();
}

bool PlayerTaskBoard::deserialize(const std::string &data) {
	std::istringstream in(data);
	std::string magic;
	in >> magic;
	if (magic != "TB1") {
		return false;
	}
	auto &s = m_state;
	int64_t v = 0;
	auto rd = [&in, &v]() -> int64_t {
		in >> v;
		return v;
	};
	s.difficulty = static_cast<uint8_t>(std::min<int64_t>(rd(), TaskBoard::DIFFICULTY_COUNT - 1));
	s.offersGenerated = rd() != 0;
	s.activeIndex = static_cast<int8_t>(rd());
	s.rerollTokens = static_cast<uint8_t>(std::min<int64_t>(rd(), TaskBoard::REROLL_TOKEN_CAP));
	s.bountyPoints = static_cast<uint32_t>(rd());
	s.nextDailyClaim = rd();
	for (auto &lvl : s.talisman) {
		lvl = static_cast<uint8_t>(rd());
	}
	for (auto &slot : s.slots) {
		slot.unlocked = rd() != 0;
		slot.preferred = static_cast<uint16_t>(rd());
		slot.unwanted = static_cast<uint16_t>(rd());
	}
	s.slots[0].unlocked = true;
	for (auto &o : s.offers) {
		o.raceId = static_cast<uint16_t>(rd());
		o.total = static_cast<uint16_t>(rd());
		o.current = static_cast<uint16_t>(rd());
		o.xp = static_cast<uint32_t>(rd());
		o.points = static_cast<uint8_t>(rd());
		o.rarity = static_cast<uint8_t>(rd());
		o.state = static_cast<uint8_t>(rd());
	}
	s.weeklyStarted = rd() != 0;
	s.weeklyFinished = rd() != 0;
	s.weeklyDifficulty = static_cast<uint8_t>(std::min<int64_t>(rd(), TaskBoard::DIFFICULTY_COUNT - 1));
	s.weeklyReset = rd();
	s.anyTotal = static_cast<uint16_t>(rd());
	s.anyCurrent = static_cast<uint16_t>(rd());
	s.anyRewarded = rd() != 0;
	s.lastPoints = static_cast<uint32_t>(rd());
	s.lastSeals = static_cast<uint32_t>(rd());
	const auto killCount = static_cast<size_t>(rd());
	if (killCount > 64) {
		return false;
	}
	s.weeklyKills.clear();
	for (size_t i = 0; i < killCount; ++i) {
		TaskBoard::WeeklyKillTask k;
		k.raceId = static_cast<uint16_t>(rd());
		k.total = static_cast<uint16_t>(rd());
		k.current = static_cast<uint16_t>(rd());
		k.rewarded = rd() != 0;
		s.weeklyKills.push_back(k);
	}
	const auto delCount = static_cast<size_t>(rd());
	if (delCount > 64) {
		return false;
	}
	s.weeklyDeliveries.clear();
	for (size_t i = 0; i < delCount; ++i) {
		TaskBoard::WeeklyDeliveryTask d;
		d.itemId = static_cast<uint16_t>(rd());
		d.total = static_cast<uint32_t>(rd());
		d.claimed = rd() != 0;
		s.weeklyDeliveries.push_back(d);
	}
	s.soulseals = static_cast<uint32_t>(rd());
	s.promotionBought = static_cast<uint8_t>(std::min<int64_t>(rd(), TaskBoard::PROMOTION_POINTS_MAX));
	s.weeklyExpansion = rd() != 0;
	const auto boughtCount = static_cast<size_t>(rd());
	if (boughtCount > 128) {
		return false;
	}
	s.shopBought.clear();
	for (size_t i = 0; i < boughtCount; ++i) {
		s.shopBought.push_back(static_cast<uint8_t>(rd()));
	}
	s.pointsSpent = static_cast<uint64_t>(rd());
	for (auto &c : s.completedTasks) {
		c = static_cast<uint32_t>(rd());
	}
	return !in.fail();
}

void PlayerTaskBoard::onLogin() {
	ensureLoaded();
	// The Permanent Weekly Task Expansion replaces the Permanent Hunting Task Slot: owners are converted automatically
	if (!m_state.weeklyExpansion) {
		if (const auto &slot = m_player.getTaskHuntingSlotById(PreySlot_Three); slot && slot->state != PreyTaskDataState_Locked) {
			m_state.weeklyExpansion = true;
			save();
		}
	}
	if (!m_state.offersGenerated) {
		generateOffers();
		save();
	}
	sendResources();
}

// ---------------------------------------------------------------------------------------
// Client requests
// ---------------------------------------------------------------------------------------
void PlayerTaskBoard::handleAction(uint8_t option, NetworkMessage &msg) {
	ensureLoaded();
	switch (option) {
		case 0: // open bounty
			sendResources();
			sendBountyData();
			break;
		case 1: // open weekly
			sendWeeklyData();
			break;
		case 2:
			changeDifficulty(msg.getByte());
			break;
		case 3:
			reroll();
			break;
		case 4:
			claimDaily();
			break;
		case 5:
			selectTask(msg.getByte());
			break;
		case 6:
			claimReward();
			break;
		case 7:
			upgradeTalisman(msg.getByte());
			break;
		case 8:
			deliverItem(msg.getByte());
			break;
		case 9:
			selectWeeklyDifficulty(msg.getByte());
			break;
		case 10:
			sendShopData();
			break;
		case 11: {
			const uint8_t index = msg.getByte();
			msg.getByte();
			buyOffer(index);
			break;
		}
		case 12:
			unlockSlot(msg.get<uint16_t>());
			break;
		case 13:
			clearPreferred(msg.get<uint16_t>(), false);
			break;
		case 14:
			clearPreferred(msg.get<uint16_t>(), true);
			break;
		case 15: {
			const auto slot = msg.get<uint16_t>();
			const auto raceId = msg.get<uint16_t>();
			assignPreferred(slot, raceId, false);
			break;
		}
		case 16: {
			const auto slot = msg.get<uint16_t>();
			const auto raceId = msg.get<uint16_t>();
			assignPreferred(slot, raceId, true);
			break;
		}
		default:
			g_logger().debug("[PlayerTaskBoard::handleAction] unknown option {} from {}", option, m_player.getName());
			break;
	}
}

// ---------------------------------------------------------------------------------------
// Bounty
// ---------------------------------------------------------------------------------------
uint32_t PlayerTaskBoard::computeBountyXp(uint16_t raceId, uint16_t total, uint8_t rarity) const {
	const double monsterExp = TaskBoard::Catalog::getInstance().getMonsterExperience(raceId);
	const double perKill = std::max(15.0, monsterExp * 0.154);
	const uint64_t mult = rarity == 2 ? 4 : (rarity == 1 ? 2 : 1);
	return clampU32(static_cast<uint64_t>(std::llround(perKill * total)) * mult);
}

TaskBoard::BountyOffer PlayerTaskBoard::makeOffer(uint8_t difficulty, uint16_t raceId) const {
	TaskBoard::BountyOffer offer;
	offer.raceId = raceId;
	const auto &range = TaskBoard::BOUNTY_KILL_RANGE[difficulty];
	offer.total = static_cast<uint16_t>(uniform_random(range.first, range.second));
	const auto roll = uniform_random(1, 100);
	offer.rarity = roll <= 2 ? 2 : (roll <= 10 ? 1 : 0);
	const uint32_t mult = offer.rarity == 2 ? 4 : (offer.rarity == 1 ? 2 : 1);
	offer.points = static_cast<uint8_t>(std::min<uint32_t>(TaskBoard::BOUNTY_POINTS[difficulty] * mult, 255));
	offer.xp = computeBountyXp(raceId, offer.total, offer.rarity);
	offer.current = 0;
	offer.state = 0;
	return offer;
}

void PlayerTaskBoard::generateOffers() {
	const auto &pool = TaskBoard::Catalog::getInstance().getPool(m_state.difficulty);

	std::vector<uint16_t> preferred;
	std::set<uint16_t> unwanted;
	for (const auto &slot : m_state.slots) {
		if (!slot.unlocked) {
			continue;
		}
		if (slot.preferred != 0) {
			preferred.push_back(slot.preferred);
		}
		if (slot.unwanted != 0) {
			unwanted.insert(slot.unwanted);
		}
	}

	std::vector<uint16_t> candidates;
	for (const auto raceId : pool) {
		if (!unwanted.contains(raceId)) {
			candidates.push_back(raceId);
		}
	}
	if (candidates.empty()) {
		candidates = pool;
	}

	std::vector<uint16_t> picked;
	for (uint8_t i = 0; i < TaskBoard::BOUNTY_OFFERS; ++i) {
		uint16_t pick = 0;
		std::vector<uint16_t> preferredAvail;
		for (const auto raceId : preferred) {
			if (std::ranges::find(candidates, raceId) != candidates.end() && std::ranges::find(picked, raceId) == picked.end()) {
				preferredAvail.push_back(raceId);
			}
		}
		if (!preferredAvail.empty() && boolean_random(0.5)) {
			pick = preferredAvail[static_cast<size_t>(uniform_random(0, static_cast<int32_t>(preferredAvail.size()) - 1))];
		} else {
			std::vector<uint16_t> avail;
			for (const auto raceId : candidates) {
				if (std::ranges::find(picked, raceId) == picked.end()) {
					avail.push_back(raceId);
				}
			}
			if (avail.empty()) {
				avail = candidates;
			}
			if (!avail.empty()) {
				pick = avail[static_cast<size_t>(uniform_random(0, static_cast<int32_t>(avail.size()) - 1))];
			}
		}
		picked.push_back(pick);
		m_state.offers[i] = pick != 0 ? makeOffer(m_state.difficulty, pick) : TaskBoard::BountyOffer {};
	}
	m_state.activeIndex = -1;
	m_state.offersGenerated = true;
}

uint8_t PlayerTaskBoard::getDailyMode() const {
	return nowSeconds() >= m_state.nextDailyClaim ? 0 : 1;
}

uint32_t PlayerTaskBoard::getTalismanCost(uint8_t path) const {
	const auto level = m_state.talisman[path];
	if (level >= TaskBoard::getTalismanMaxLevel(path)) {
		return 0;
	}
	const int64_t step = static_cast<int64_t>(level) - TaskBoard::getTalismanStartLevel(path) + 1;
	return static_cast<uint32_t>(std::max<int64_t>(5, 12 * step - 7));
}

void PlayerTaskBoard::changeDifficulty(uint8_t difficulty) {
	difficulty = std::min<uint8_t>(difficulty, TaskBoard::DIFFICULTY_COUNT - 1);
	// Like real Tibia ("You can change the difficulty at any time, but it only takes effect when selecting your next task"):
	// switching only stores the difficulty. The offers on screen stay, so switching cannot be used as a free reroll.
	if (difficulty != m_state.difficulty) {
		m_state.difficulty = difficulty;
		save();
	}
	sendBountyData();
}

void PlayerTaskBoard::selectTask(uint8_t index) {
	if (index >= TaskBoard::BOUNTY_OFFERS || m_state.offers[index].raceId == 0 || m_state.offers[index].state != 0) {
		sendBountyData();
		return;
	}
	if (m_state.activeIndex >= 0) {
		m_player.sendCancelMessage("You already have an active bounty task.");
		sendBountyData();
		return;
	}
	m_state.offers[index].state = 1;
	m_state.offers[index].current = 0;
	m_state.activeIndex = static_cast<int8_t>(index);
	save();
	sendBountyData();
}

void PlayerTaskBoard::claimReward() {
	if (m_state.activeIndex < 0 || m_state.activeIndex >= TaskBoard::BOUNTY_OFFERS) {
		sendBountyData();
		return;
	}
	auto &offer = m_state.offers[m_state.activeIndex];
	if (offer.state != 2) {
		sendBountyData();
		return;
	}
	const uint8_t rewardPoints = offer.points;
	const uint32_t rewardXp = offer.xp;
	offer.state = 3;
	m_state.activeIndex = -1;
	m_state.bountyPoints += rewardPoints;
	m_state.rerollTokens = static_cast<uint8_t>(std::min<int>(TaskBoard::REROLL_TOKEN_CAP, m_state.rerollTokens + 1));

	const bool anyLeft = std::ranges::any_of(m_state.offers, [](const TaskBoard::BountyOffer &o) { return o.raceId != 0 && o.state == 0; });
	if (!anyLeft) {
		generateOffers();
	}
	m_player.sendTextMessage(MESSAGE_EVENT_ADVANCE, fmt::format("You have claimed the reward of your bounty task: {} bounty points and 1 reroll token.", rewardPoints));
	if (rewardXp > 0) {
		m_player.addExperience(nullptr, rewardXp, true);
	}
	save();
	sendResources();
	sendBountyData();
}

void PlayerTaskBoard::reroll() {
	if (m_state.activeIndex >= 0) {
		m_player.sendCancelMessage("You cannot reroll while a bounty task is active.");
		sendBountyData();
		return;
	}
	if (m_state.rerollTokens == 0) {
		m_player.sendCancelMessage("You do not have any reroll tokens.");
		sendBountyData();
		return;
	}
	m_state.rerollTokens--;
	generateOffers();
	save();
	sendBountyData();
}

void PlayerTaskBoard::claimDaily() {
	if (getDailyMode() != 0) {
		m_player.sendCancelMessage("You have already claimed your daily reroll token.");
		sendBountyData();
		return;
	}
	m_state.rerollTokens = static_cast<uint8_t>(std::min<int>(TaskBoard::REROLL_TOKEN_CAP, m_state.rerollTokens + 1));
	m_state.nextDailyClaim = nextDailyReset(nowSeconds());
	save();
	sendBountyData();
}

void PlayerTaskBoard::upgradeTalisman(uint8_t path) {
	if (path >= TaskBoard::TALISMAN_PATHS) {
		return;
	}
	const auto cost = getTalismanCost(path);
	if (cost == 0) {
		sendBountyData();
		return;
	}
	if (m_state.bountyPoints < cost) {
		m_player.sendCancelMessage("You do not have enough Bounty Points.");
		sendBountyData();
		return;
	}
	m_state.bountyPoints -= cost;
	m_state.talisman[path]++;
	save();
	sendResources();
	sendBountyData();
}

void PlayerTaskBoard::unlockSlot(uint16_t slot) {
	if (slot < 2 || slot > TaskBoard::PREFERRED_SLOTS || m_state.slots[slot - 1].unlocked) {
		sendBountyData();
		return;
	}
	const uint32_t cost = 300u * (slot - 1u);
	if (m_state.bountyPoints < cost) {
		m_player.sendCancelMessage("You do not have enough Bounty Points.");
		sendBountyData();
		return;
	}
	m_state.bountyPoints -= cost;
	m_state.slots[slot - 1].unlocked = true;
	save();
	sendResources();
	sendBountyData();
}

void PlayerTaskBoard::assignPreferred(uint16_t slot, uint16_t raceId, bool unwanted) {
	if (slot < 1 || slot > TaskBoard::PREFERRED_SLOTS || !m_state.slots[slot - 1].unlocked) {
		sendBountyData();
		return;
	}
	const auto &mType = g_monsters().getMonsterTypeByRaceId(raceId);
	if (!mType || mType->isBoss()) {
		sendBountyData();
		return;
	}
	if (m_state.bountyPoints < TaskBoard::PREFERRED_ASSIGN_COST) {
		m_player.sendCancelMessage("You do not have enough Bounty Points.");
		sendBountyData();
		return;
	}
	auto &entry = m_state.slots[slot - 1];
	// a creature cannot be preferred and unwanted at the same time
	for (auto &other : m_state.slots) {
		if (unwanted && other.preferred == raceId) {
			other.preferred = 0;
		}
		if (!unwanted && other.unwanted == raceId) {
			other.unwanted = 0;
		}
	}
	if (unwanted) {
		entry.unwanted = raceId;
	} else {
		entry.preferred = raceId;
	}
	m_state.bountyPoints -= TaskBoard::PREFERRED_ASSIGN_COST;
	save();
	sendResources();
	sendBountyData();
}

void PlayerTaskBoard::clearPreferred(uint16_t slot, bool unwanted) {
	if (slot < 1 || slot > TaskBoard::PREFERRED_SLOTS || !m_state.slots[slot - 1].unlocked) {
		sendBountyData();
		return;
	}
	auto &entry = m_state.slots[slot - 1];
	if ((unwanted ? entry.unwanted : entry.preferred) == 0) {
		sendBountyData();
		return;
	}
	if (m_state.bountyPoints < TaskBoard::PREFERRED_ASSIGN_COST) {
		m_player.sendCancelMessage("You do not have enough Bounty Points.");
		sendBountyData();
		return;
	}
	if (unwanted) {
		entry.unwanted = 0;
	} else {
		entry.preferred = 0;
	}
	m_state.bountyPoints -= TaskBoard::PREFERRED_ASSIGN_COST;
	save();
	sendResources();
	sendBountyData();
}

bool PlayerTaskBoard::isActiveBountyRace(uint16_t raceId) {
	ensureLoaded();
	if (raceId == 0 || m_state.activeIndex < 0 || m_state.activeIndex >= TaskBoard::BOUNTY_OFFERS) {
		return false;
	}
	const auto &offer = m_state.offers[m_state.activeIndex];
	return offer.state == 1 && offer.raceId == raceId;
}

bool PlayerTaskBoard::hasBountyTalismanEquipped() const {
	// the Bounty Talisman sits in the slot below the shield (the ammunition slot)
	const auto talisman = m_player.getInventoryItem(CONST_SLOT_AMMO);
	return talisman && talisman->getID() == TaskBoard::BOUNTY_TALISMAN_ITEM_ID;
}

// Soul Eater: like real Tibia, the talisman bonuses only apply while the Bounty Talisman is worn in the ring slot.
double PlayerTaskBoard::getDamagePercent(uint16_t raceId) {
	return hasBountyTalismanEquipped() && isActiveBountyRace(raceId) ? TaskBoard::getTalismanBonusHundredths(m_state.talisman[0], 0) / 100.0 : 0.0;
}

double PlayerTaskBoard::getLifeLeechPercent(uint16_t raceId) {
	return hasBountyTalismanEquipped() && isActiveBountyRace(raceId) ? TaskBoard::getTalismanBonusHundredths(m_state.talisman[1], 1) / 100.0 : 0.0;
}

double PlayerTaskBoard::getLootPercent(uint16_t raceId) {
	return hasBountyTalismanEquipped() && isActiveBountyRace(raceId) ? TaskBoard::getTalismanBonusHundredths(m_state.talisman[2], 2) / 100.0 : 0.0;
}

double PlayerTaskBoard::getDoubleBestiaryPercent(uint16_t raceId) {
	return hasBountyTalismanEquipped() && isActiveBountyRace(raceId) ? TaskBoard::getTalismanBonusHundredths(m_state.talisman[3], 3) / 100.0 : 0.0;
}

// ---------------------------------------------------------------------------------------
// Weekly
// ---------------------------------------------------------------------------------------
uint8_t PlayerTaskBoard::killSlotCount() const {
	return m_state.weeklyExpansion ? 9 : 6;
}

uint8_t PlayerTaskBoard::deliverySlotCount() const {
	return m_state.weeklyExpansion ? 9 : 6;
}

uint8_t PlayerTaskBoard::unlockedDifficulty() const {
	uint8_t unlocked = 0;
	for (uint8_t d = 0; d < TaskBoard::DIFFICULTY_COUNT; ++d) {
		if (m_player.getLevel() >= TaskBoard::WEEKLY_MIN_LEVEL[d]) {
			unlocked = d;
		}
	}
	return unlocked;
}

uint64_t PlayerTaskBoard::killTaskExperience() const {
	const auto xp = TaskBoard::getLevelExperienceReward(m_player.getLevel());
	const auto cap = TaskBoard::WEEKLY_KILL_XP_CAP[m_state.weeklyDifficulty];
	return cap == 0 ? xp : std::min(xp, cap);
}

uint32_t PlayerTaskBoard::countCompletedKillTasks() const {
	uint32_t count = 0;
	if (m_state.anyTotal > 0 && m_state.anyCurrent >= m_state.anyTotal) {
		count++;
	}
	for (const auto &k : m_state.weeklyKills) {
		if (k.total > 0 && k.current >= k.total) {
			count++;
		}
	}
	return count;
}

uint32_t PlayerTaskBoard::countCompletedDeliveryTasks() const {
	return static_cast<uint32_t>(std::ranges::count_if(m_state.weeklyDeliveries, [](const TaskBoard::WeeklyDeliveryTask &d) { return d.claimed; }));
}

// 2x for 4 tasks, 3x for 8, 5x for 12, 8x for 16 (Tibia wiki)
uint32_t PlayerTaskBoard::rewardMultiplier(uint32_t completed) const {
	if (completed >= 16) {
		return 8;
	}
	if (completed >= 12) {
		return 5;
	}
	if (completed >= 8) {
		return 3;
	}
	if (completed >= 4) {
		return 2;
	}
	return 1;
}

uint32_t PlayerTaskBoard::projectedPoints() const {
	const auto kills = countCompletedKillTasks();
	const auto deliveries = countCompletedDeliveryTasks();
	const uint32_t base = kills * TaskBoard::WEEKLY_KILL_POINTS[m_state.weeklyDifficulty] + deliveries * TaskBoard::WEEKLY_DELIVERY_POINTS;
	return base * rewardMultiplier(kills + deliveries);
}

void PlayerTaskBoard::checkWeeklyRollover() {
	if (m_loaded && m_state.weeklyStarted && !m_state.weeklyFinished && nowSeconds() >= m_state.weeklyReset) {
		finishWeek();
	}
}

void PlayerTaskBoard::finishWeek() {
	const auto points = projectedPoints();
	const auto seals = countCompletedKillTasks() + countCompletedDeliveryTasks();
	m_state.lastPoints = points;
	m_state.lastSeals = seals;
	m_state.weeklyFinished = true;
	m_state.soulseals += seals;
	if (points > 0) {
		m_player.addTaskHuntingPoints(points);
	}
	save();
	if (points > 0 || seals > 0) {
		m_player.sendTextMessage(MESSAGE_EVENT_ADVANCE, fmt::format("Your weekly tasks have ended. You received {} Hunting Task Points and {} Soulseals.", points, seals));
	}
	sendResources();
}

void PlayerTaskBoard::selectWeeklyDifficulty(uint8_t difficulty) {
	difficulty = std::min<uint8_t>(difficulty, TaskBoard::DIFFICULTY_COUNT - 1);
	if (m_state.weeklyStarted && !m_state.weeklyFinished) {
		sendWeeklyData();
		return;
	}
	if (m_player.getLevel() < TaskBoard::WEEKLY_MIN_LEVEL[difficulty]) {
		m_player.sendCancelMessage(fmt::format("The minimum level to start this difficulty is {}.", TaskBoard::WEEKLY_MIN_LEVEL[difficulty]));
		sendWeeklyData();
		return;
	}

	auto &catalog = TaskBoard::Catalog::getInstance();
	const auto &pool = catalog.getPool(difficulty);
	const auto &deliveryPool = catalog.getDeliveryPool(difficulty);
	if (pool.empty()) {
		m_player.sendCancelMessage("There are no weekly tasks available for this difficulty.");
		sendWeeklyData();
		return;
	}

	m_state.weeklyStarted = true;
	m_state.weeklyFinished = false;
	m_state.weeklyDifficulty = difficulty;
	m_state.weeklyReset = nextWeeklyReset(nowSeconds());
	m_state.lastPoints = 0;
	m_state.lastSeals = 0;
	static constexpr std::array<uint16_t, 4> anyTotals = { 1000, 1500, 2000, 3000 };
	static constexpr std::array<std::pair<int32_t, int32_t>, 4> killRanges = { { { 25, 60 }, { 50, 120 }, { 100, 250 }, { 200, 400 } } };
	m_state.anyTotal = anyTotals[difficulty];
	m_state.anyCurrent = 0;
	m_state.anyRewarded = false;
	m_state.weeklyKills.clear();
	m_state.weeklyDeliveries.clear();

	// Kill tasks: "any creature" + specific creatures
	std::vector<uint16_t> shuffled = pool;
	for (size_t i = shuffled.size(); i > 1; --i) {
		std::swap(shuffled[i - 1], shuffled[static_cast<size_t>(uniform_random(0, static_cast<int32_t>(i) - 1))]);
	}
	const size_t specific = std::min<size_t>(killSlotCount() - 1u, shuffled.size());
	for (size_t i = 0; i < specific; ++i) {
		TaskBoard::WeeklyKillTask task;
		task.raceId = shuffled[i];
		task.total = static_cast<uint16_t>(uniform_random(killRanges[difficulty].first, killRanges[difficulty].second));
		m_state.weeklyKills.push_back(task);
	}

	// Delivery tasks
	std::vector<TaskBoard::Catalog::DeliveryCandidate> items = deliveryPool;
	for (size_t i = items.size(); i > 1; --i) {
		std::swap(items[i - 1], items[static_cast<size_t>(uniform_random(0, static_cast<int32_t>(i) - 1))]);
	}
	const size_t deliveries = std::min<size_t>(deliverySlotCount(), items.size());
	for (size_t i = 0; i < deliveries; ++i) {
		const auto &cand = items[i];
		const double dropRate = std::max(0.02, cand.chance / 100000.0) * std::max<uint32_t>(1, cand.avgCount);
		const double killsWorth = static_cast<double>(uniform_random(20, 40));
		TaskBoard::WeeklyDeliveryTask task;
		task.itemId = cand.itemId;
		task.total = static_cast<uint32_t>(std::clamp<double>(std::round(killsWorth * dropRate), 5.0, 250.0));
		m_state.weeklyDeliveries.push_back(task);
	}

	save();
	sendWeeklyData();
}

uint32_t PlayerTaskBoard::getDeliveryCurrent(const TaskBoard::WeeklyDeliveryTask &task) const {
	if (task.claimed) {
		return task.total;
	}
	const uint64_t owned = static_cast<uint64_t>(m_player.getItemTypeCount(task.itemId)) + m_player.getStashItemCount(task.itemId);
	return static_cast<uint32_t>(std::min<uint64_t>(owned, task.total));
}

void PlayerTaskBoard::deliverItem(uint8_t slot) {
	if (!m_state.weeklyStarted || m_state.weeklyFinished || slot >= m_state.weeklyDeliveries.size()) {
		sendWeeklyData();
		return;
	}
	auto &task = m_state.weeklyDeliveries[slot];
	if (task.claimed) {
		sendWeeklyData();
		return;
	}
	if (getDeliveryCurrent(task) < task.total || !m_player.removeItemCountById(task.itemId, task.total, true)) {
		m_player.sendCancelMessage("You do not have enough items to deliver.");
		sendWeeklyData();
		return;
	}
	task.claimed = true;
	const auto xp = TaskBoard::getLevelExperienceReward(m_player.getLevel());
	m_player.sendTextMessage(MESSAGE_EVENT_ADVANCE, "You have completed a weekly delivery task.");
	m_player.addExperience(nullptr, xp, true);
	onWeeklyTaskCompleted();
	save();
	sendWeeklyData();
}

void PlayerTaskBoard::completeKillTask(TaskBoard::WeeklyKillTask &task) {
	if (task.rewarded) {
		return;
	}
	task.rewarded = true;
	m_player.sendTextMessage(MESSAGE_EVENT_ADVANCE, "You have completed a weekly kill task.");
	m_player.addExperience(nullptr, killTaskExperience(), true);
	onWeeklyTaskCompleted();
}

void PlayerTaskBoard::onWeeklyTaskCompleted() {
	const auto tier = m_state.weeklyDifficulty >= 2 ? 2 : m_state.weeklyDifficulty;
	m_state.completedTasks[tier]++;
	// Verminbane: 100 beginner tasks, Monsterhunter: 200 adept tasks, Taskmaster: 500 expert/master tasks
	if (m_state.completedTasks[0] >= 100) {
		m_player.achiev().add(502);
	}
	if (m_state.completedTasks[1] >= 200) {
		m_player.achiev().add(503);
	}
	if (m_state.completedTasks[2] >= 500) {
		m_player.achiev().add(504);
	}
}

void PlayerTaskBoard::checkShopAchievements() {
	const auto &shop = TaskBoard::Catalog::getInstance().getShop();
	bool base = false;
	bool addon1 = false;
	bool addon2 = false;
	for (size_t i = 0; i < shop.size(); ++i) {
		if (!isOfferBought(i)) {
			continue;
		}
		if (shop[i].type == TaskBoard::SHOP_OUTFIT) {
			base = base || shop[i].addons == 0;
			addon1 = addon1 || shop[i].addons == 1;
			addon2 = addon2 || shop[i].addons == 2;
		} else if (shop[i].type == TaskBoard::SHOP_MOUNT) {
			m_player.achiev().add(507); // Steppe Elegance (Antelope)
		}
	}
	if (base && addon1 && addon2) {
		m_player.achiev().add(506); // Falconer
	}
}

void PlayerTaskBoard::setWeeklyExpansion(bool value) {
	ensureLoaded();
	m_state.weeklyExpansion = value;
	if (value && m_state.weeklyStarted && !m_state.weeklyFinished) {
		// top up the current week with the additional slots
		auto &catalog = TaskBoard::Catalog::getInstance();
		const auto difficulty = m_state.weeklyDifficulty;
		static constexpr std::array<std::pair<int32_t, int32_t>, 4> killRanges = { { { 25, 60 }, { 50, 120 }, { 100, 250 }, { 200, 400 } } };
		std::vector<uint16_t> shuffled = catalog.getPool(difficulty);
		for (size_t i = shuffled.size(); i > 1; --i) {
			std::swap(shuffled[i - 1], shuffled[static_cast<size_t>(uniform_random(0, static_cast<int32_t>(i) - 1))]);
		}
		for (const auto raceId : shuffled) {
			if (m_state.weeklyKills.size() >= killSlotCount() - 1u) {
				break;
			}
			const bool used = std::ranges::any_of(m_state.weeklyKills, [raceId](const TaskBoard::WeeklyKillTask &k) { return k.raceId == raceId; });
			if (used) {
				continue;
			}
			TaskBoard::WeeklyKillTask task;
			task.raceId = raceId;
			task.total = static_cast<uint16_t>(uniform_random(killRanges[difficulty].first, killRanges[difficulty].second));
			m_state.weeklyKills.push_back(task);
		}
		std::vector<TaskBoard::Catalog::DeliveryCandidate> items = catalog.getDeliveryPool(difficulty);
		for (size_t i = items.size(); i > 1; --i) {
			std::swap(items[i - 1], items[static_cast<size_t>(uniform_random(0, static_cast<int32_t>(i) - 1))]);
		}
		for (const auto &cand : items) {
			if (m_state.weeklyDeliveries.size() >= deliverySlotCount()) {
				break;
			}
			const bool used = std::ranges::any_of(m_state.weeklyDeliveries, [&cand](const TaskBoard::WeeklyDeliveryTask &d) { return d.itemId == cand.itemId; });
			if (used) {
				continue;
			}
			const double dropRate = std::max(0.02, cand.chance / 100000.0) * std::max<uint32_t>(1, cand.avgCount);
			TaskBoard::WeeklyDeliveryTask task;
			task.itemId = cand.itemId;
			task.total = static_cast<uint32_t>(std::clamp<double>(std::round(static_cast<double>(uniform_random(20, 40)) * dropRate), 5.0, 250.0));
			m_state.weeklyDeliveries.push_back(task);
		}
	}
	save();
	// refresh an open Task Board right away, so the new slots show up without closing and reopening it
	if (value) {
		sendWeeklyData();
	}
}

// ---------------------------------------------------------------------------------------
// Kills
// ---------------------------------------------------------------------------------------
void PlayerTaskBoard::onMonsterKilled(const std::shared_ptr<MonsterType> &mType) {
	if (!mType) {
		return;
	}
	ensureLoaded();
	const auto raceId = mType->info.raceid;
	bool bountyChanged = false;
	bool weeklyChanged = false;

	// Bounty task (summons are filtered by the caller)
	if (raceId != 0 && m_state.activeIndex >= 0 && m_state.activeIndex < TaskBoard::BOUNTY_OFFERS) {
		auto &offer = m_state.offers[m_state.activeIndex];
		if (offer.state == 1 && offer.raceId == raceId) {
			offer.current++;
			bountyChanged = true;
			if (offer.current >= offer.total) {
				offer.current = offer.total;
				offer.state = 2;
				m_player.sendTextMessage(MESSAGE_EVENT_ADVANCE, "You have completed your bounty task! Open the Task Board to claim your reward.");
			}
		}
	}

	// Weekly kill tasks
	if (m_state.weeklyStarted && !m_state.weeklyFinished && nowSeconds() < m_state.weeklyReset) {
		if (m_state.anyTotal > 0 && m_state.anyCurrent < m_state.anyTotal) {
			m_state.anyCurrent++;
			weeklyChanged = true;
			if (m_state.anyCurrent >= m_state.anyTotal && !m_state.anyRewarded) {
				m_state.anyRewarded = true;
				m_player.sendTextMessage(MESSAGE_EVENT_ADVANCE, "You have completed a weekly kill task.");
				m_player.addExperience(nullptr, killTaskExperience(), true);
				onWeeklyTaskCompleted();
			}
		}
		if (raceId != 0) {
			for (auto &task : m_state.weeklyKills) {
				if (task.raceId == raceId && task.current < task.total) {
					task.current++;
					weeklyChanged = true;
					if (task.current >= task.total) {
						completeKillTask(task);
					}
				}
			}
		}
	}

	if (bountyChanged || weeklyChanged) {
		save();
		if (bountyChanged) {
			sendBountyData();
		}
		if (weeklyChanged) {
			sendWeeklyData();
		}
	}
}

// ---------------------------------------------------------------------------------------
// Shop
// ---------------------------------------------------------------------------------------
uint32_t PlayerTaskBoard::promotionCost(uint8_t alreadyBought) const {
	const uint32_t n = static_cast<uint32_t>(alreadyBought) + 1;
	return 100 + 50 * (n - 1) * n;
}

bool PlayerTaskBoard::isOfferBought(size_t index) const {
	return std::ranges::find(m_state.shopBought, static_cast<uint8_t>(index)) != m_state.shopBought.end();
}

uint8_t PlayerTaskBoard::shopStatus(size_t index, const TaskBoard::ShopOffer &offer) const {
	// 0 available, 1 not available, 2 not enough points, 3 requires outfit, 4 bought
	if (offer.type == TaskBoard::SHOP_PROMOTION) {
		if (m_state.promotionBought >= TaskBoard::PROMOTION_POINTS_MAX) {
			return 4;
		}
		return m_player.getTaskHuntingPoints() >= promotionCost(m_state.promotionBought) ? 0 : 2;
	}
	if (offer.type == TaskBoard::SHOP_OUTFIT || offer.type == TaskBoard::SHOP_MOUNT) {
		if (isOfferBought(index)) {
			return 4;
		}
		if (offer.type == TaskBoard::SHOP_MOUNT) {
			const auto mount = g_game().mounts->getMountByID(static_cast<uint8_t>(offer.mountId));
			if (mount && m_player.hasMount(mount)) {
				return 4;
			}
		}
		if (offer.type == TaskBoard::SHOP_OUTFIT && offer.addons > 0) {
			const auto look = m_player.getSex() == PLAYERSEX_FEMALE ? offer.lookTypeFemale : offer.lookTypeMale;
			bool baseOwned = false;
			const auto &shop = TaskBoard::Catalog::getInstance().getShop();
			for (size_t i = 0; i < shop.size(); ++i) {
				if (shop[i].type == TaskBoard::SHOP_OUTFIT && shop[i].addons == 0 && shop[i].lookTypeMale == offer.lookTypeMale && isOfferBought(i)) {
					baseOwned = true;
				}
			}
			if (!baseOwned && !m_player.canWear(look, 0)) {
				return 3;
			}
		}
	}
	return m_player.getTaskHuntingPoints() >= offer.price ? 0 : 2;
}

void PlayerTaskBoard::buyOffer(uint8_t index) {
	const auto &shop = TaskBoard::Catalog::getInstance().getShop();
	if (index >= shop.size()) {
		m_player.sendCancelMessage("This offer is not available.");
		return;
	}
	const auto &offer = shop[index];
	const auto status = shopStatus(index, offer);
	if (status == 4) {
		m_player.sendCancelMessage("You have already purchased this offer.");
		sendShopData();
		return;
	}
	if (status == 3) {
		m_player.sendCancelMessage("You need the base outfit first.");
		sendShopData();
		return;
	}
	if (status != 0) {
		m_player.sendCancelMessage("You do not have enough Hunting Task Points.");
		sendShopData();
		return;
	}

	switch (offer.type) {
		case TaskBoard::SHOP_PROMOTION: {
			const auto cost = promotionCost(m_state.promotionBought);
			if (!m_player.useTaskHuntingPoints(cost)) {
				return;
			}
			m_state.pointsSpent += cost;
			m_state.promotionBought++;
			m_player.sendTextMessage(MESSAGE_EVENT_ADVANCE, "You have purchased a Bonus Promotion Point for your Wheel of Destiny.");
			break;
		}
		case TaskBoard::SHOP_OUTFIT: {
			if (!m_player.useTaskHuntingPoints(offer.price)) {
				return;
			}
			m_state.pointsSpent += offer.price;
			const uint8_t addonMask = offer.addons == 0 ? 0 : static_cast<uint8_t>(1 << (offer.addons - 1));
			m_player.addOutfit(offer.lookTypeMale, addonMask);
			m_player.addOutfit(offer.lookTypeFemale, addonMask);
			m_state.shopBought.push_back(index);
			m_player.sendTextMessage(MESSAGE_EVENT_ADVANCE, fmt::format("You have purchased {}.", offer.title));
			break;
		}
		case TaskBoard::SHOP_MOUNT: {
			if (!m_player.useTaskHuntingPoints(offer.price)) {
				return;
			}
			m_state.pointsSpent += offer.price;
			m_player.tameMount(static_cast<uint8_t>(offer.mountId));
			m_state.shopBought.push_back(index);
			m_player.sendTextMessage(MESSAGE_EVENT_ADVANCE, fmt::format("You have purchased {}.", offer.title));
			break;
		}
		default: {
			const auto inbox = m_player.getStoreInbox();
			if (!inbox) {
				m_player.sendCancelMessage("Your store inbox is not available.");
				return;
			}
			if (!m_player.useTaskHuntingPoints(offer.price)) {
				return;
			}
			const auto [ret, a, b] = g_game().createItem(inbox, offer.itemId, 1, 0, FLAG_NOLIMIT, false);
			(void)a;
			(void)b;
			if (ret != RETURNVALUE_NOERROR) {
				m_player.addTaskHuntingPoints(offer.price);
				m_player.sendCancelMessage("The item could not be delivered to your store inbox.");
				return;
			}
			m_state.pointsSpent += offer.price;
			m_player.sendTextMessage(MESSAGE_EVENT_ADVANCE, fmt::format("You have purchased {}. It was delivered to your store inbox.", offer.title));
			break;
		}
	}
	checkShopAchievements();
	m_player.title().checkAndUpdateNewTitles();
	save();
	sendResources();
	sendShopData();
}

// ---------------------------------------------------------------------------------------
// Accessors
// ---------------------------------------------------------------------------------------
uint32_t PlayerTaskBoard::getBountyPoints() {
	ensureLoaded();
	return m_state.bountyPoints;
}

uint32_t PlayerTaskBoard::getSoulseals() {
	ensureLoaded();
	return m_state.soulseals;
}

uint32_t PlayerTaskBoard::getPromotionPoints() {
	ensureLoaded();
	return m_state.promotionBought;
}

uint64_t PlayerTaskBoard::getPointsSpent() {
	ensureLoaded();
	return m_state.pointsSpent;
}

bool PlayerTaskBoard::hasWeeklyExpansion() {
	ensureLoaded();
	return m_state.weeklyExpansion;
}

void PlayerTaskBoard::addBountyPoints(uint32_t amount) {
	ensureLoaded();
	m_state.bountyPoints += amount;
	save();
	sendResources();
}

void PlayerTaskBoard::addSoulseals(uint32_t amount) {
	ensureLoaded();
	m_state.soulseals += amount;
	save();
	sendResources();
}

void PlayerTaskBoard::addRerollTokens(uint8_t amount) {
	ensureLoaded();
	m_state.rerollTokens = static_cast<uint8_t>(std::min<int>(TaskBoard::REROLL_TOKEN_CAP, m_state.rerollTokens + amount));
	save();
}

// ---------------------------------------------------------------------------------------
// Packets
// ---------------------------------------------------------------------------------------
void PlayerTaskBoard::sendResources() const {
	if (m_player.client) {
		m_player.client->sendTaskBoardResource(RESOURCE_BOUNTY_POINTS, m_state.bountyPoints);
		m_player.client->sendTaskBoardResource(RESOURCE_SOULSEALS, m_state.soulseals);
	}
}

void PlayerTaskBoard::sendBountyData() {
	if (!m_player.client || m_player.client->oldProtocol) {
		return;
	}
	ensureLoaded();
	if (!m_state.offersGenerated) {
		generateOffers();
		save();
	}

	NetworkMessage msg;
	msg.addByte(0x5B);
	msg.addByte(0); // bounty

	msg.addByte(TaskBoard::BOUNTY_OFFERS);
	for (uint8_t i = 0; i < TaskBoard::BOUNTY_OFFERS; ++i) {
		const auto &o = m_state.offers[i];
		msg.addByte(i);
		msg.add<uint16_t>(o.raceId);
		msg.add<uint16_t>(o.total);
		msg.add<uint32_t>(o.xp);
		msg.addByte(o.points);
		msg.add<uint16_t>(o.current);
		msg.addByte(o.state);
		msg.addByte(o.rarity);
	}

	msg.addByte(m_state.rerollTokens);
	msg.addByte(getDailyMode());
	msg.addByte(m_state.difficulty);

	for (uint8_t path = 0; path < TaskBoard::TALISMAN_PATHS; ++path) {
		const auto cost = getTalismanCost(path);
		msg.addByte(m_state.talisman[path]);
		msg.addByte(0);
		msg.addByte(cost != 0 && m_state.bountyPoints >= cost ? 1 : 0);
		msg.add<uint16_t>(static_cast<uint16_t>(std::min<uint32_t>(cost, 65535)));
	}

	msg.addByte(TaskBoard::PREFERRED_SLOTS);
	for (const auto &slot : m_state.slots) {
		msg.addByte(slot.unlocked ? 1 : 0);
		msg.add<uint16_t>(slot.preferred);
		msg.add<uint16_t>(slot.unwanted);
	}

	m_player.client->writeToOutputBuffer(msg);
}

void PlayerTaskBoard::sendWeeklyData() {
	if (!m_player.client || m_player.client->oldProtocol) {
		return;
	}
	ensureLoaded();

	const bool active = m_state.weeklyStarted;
	NetworkMessage msg;
	msg.addByte(0x5B);
	msg.addByte(1); // weekly

	msg.add<uint16_t>(active ? m_state.anyTotal : 0);
	msg.add<uint16_t>(active ? m_state.anyCurrent : 0);

	const auto killCount = active ? m_state.weeklyKills.size() : 0;
	msg.addByte(static_cast<uint8_t>(killCount));
	for (size_t i = 0; i < killCount; ++i) {
		const auto &k = m_state.weeklyKills[i];
		msg.add<uint16_t>(k.raceId);
		msg.add<uint16_t>(k.total);
		msg.add<uint16_t>(k.current);
	}

	const auto deliveryCount = active ? m_state.weeklyDeliveries.size() : 0;
	msg.addByte(static_cast<uint8_t>(deliveryCount));
	for (size_t i = 0; i < deliveryCount; ++i) {
		const auto &d = m_state.weeklyDeliveries[i];
		msg.addByte(static_cast<uint8_t>(i));
		msg.add<uint16_t>(d.itemId);
		msg.addByte(0);
		msg.addByte(0);
		msg.add<uint32_t>(d.total);
		msg.add<uint32_t>(getDeliveryCurrent(d));
		msg.addByte(d.claimed ? 1 : 0);
	}

	const uint8_t difficulty = active ? m_state.weeklyDifficulty : 0;
	msg.addByte(difficulty);
	// experience per kill task / delivery task for the displayed difficulty
	const auto deliveryXp = TaskBoard::getLevelExperienceReward(m_player.getLevel());
	const auto previousDifficulty = m_state.weeklyDifficulty;
	uint64_t killXp = deliveryXp;
	{
		const auto cap = TaskBoard::WEEKLY_KILL_XP_CAP[previousDifficulty];
		if (cap != 0) {
			killXp = std::min(killXp, cap);
		}
	}
	msg.add<uint32_t>(clampU32(killXp));
	msg.add<uint32_t>(clampU32(deliveryXp));
	msg.addByte(static_cast<uint8_t>(active ? countCompletedKillTasks() : 0));
	msg.addByte(static_cast<uint8_t>(active ? countCompletedDeliveryTasks() : 0));
	msg.addByte(active && m_state.weeklyFinished ? 1 : 0);
	msg.addByte(unlockedDifficulty());
	msg.add<uint32_t>(active ? static_cast<uint32_t>(m_state.weeklyReset) : 0);
	msg.addByte(m_state.weeklyExpansion ? 1 : 0);
	const uint32_t points = !active ? 0 : (m_state.weeklyFinished ? m_state.lastPoints : projectedPoints());
	const uint32_t seals = !active ? 0 : (m_state.weeklyFinished ? m_state.lastSeals : (countCompletedKillTasks() + countCompletedDeliveryTasks()));
	msg.add<uint32_t>(points);
	msg.add<uint32_t>(seals);

	m_player.client->writeToOutputBuffer(msg);
}

void PlayerTaskBoard::sendShopData() {
	if (!m_player.client || m_player.client->oldProtocol) {
		return;
	}
	ensureLoaded();
	const auto &shop = TaskBoard::Catalog::getInstance().getShop();

	NetworkMessage msg;
	msg.addByte(0x5B);
	msg.addByte(2); // hunting task shop
	msg.addByte(static_cast<uint8_t>(shop.size()));
	for (size_t i = 0; i < shop.size(); ++i) {
		const auto &offer = shop[i];
		const auto status = shopStatus(i, offer);
		msg.addByte(static_cast<uint8_t>(offer.type));
		if (offer.type == TaskBoard::SHOP_PROMOTION) {
			msg.add<uint16_t>(static_cast<uint16_t>(m_state.promotionBought + 1));
			msg.add<uint32_t>(m_state.promotionBought >= TaskBoard::PROMOTION_POINTS_MAX ? 0 : promotionCost(m_state.promotionBought));
			msg.addByte(status);
			continue;
		}
		msg.addString(offer.title);
		msg.addString(offer.description);
		if (offer.type == TaskBoard::SHOP_OUTFIT) {
			msg.add<uint32_t>(m_player.getSex() == PLAYERSEX_FEMALE ? offer.lookTypeFemale : offer.lookTypeMale);
			msg.addByte(offer.addons == 0 ? 3 : (offer.addons == 1 ? 1 : 2));
		} else if (offer.type == TaskBoard::SHOP_MOUNT) {
			const auto mount = g_game().mounts->getMountByID(static_cast<uint8_t>(offer.mountId));
			msg.add<uint32_t>(mount ? mount->clientId : 0);
		} else {
			msg.add<uint32_t>(offer.itemId);
		}
		msg.add<uint32_t>(offer.price);
		msg.addByte(status);
	}

	m_player.client->writeToOutputBuffer(msg);
}
