/**
 * Canary - A free and open-source MMORPG server emulator
 * Task Board (Bounty Tasks, Weekly Tasks, Hunting Task Shop) - Soul Eater
 */

#pragma once

#include "creatures/creatures_definitions.hpp"

class Player;
class MonsterType;
class NetworkMessage;

namespace TaskBoard {
	constexpr uint8_t DIFFICULTY_COUNT = 4;
	constexpr uint8_t BOUNTY_OFFERS = 3;
	constexpr uint8_t TALISMAN_PATHS = 4;
	constexpr uint8_t PREFERRED_SLOTS = 5;
	constexpr uint8_t REROLL_TOKEN_CAP = 10;
	constexpr uint32_t PREFERRED_ASSIGN_COST = 10;
	constexpr uint8_t PROMOTION_POINTS_MAX = 50;
	// Soul Eater: the Bounty Talisman item (ring slot); its bonuses only work while it is worn
	constexpr uint16_t BOUNTY_TALISMAN_ITEM_ID = 31263;

	// Bounty kill amount range per difficulty (Tibia wiki)
	constexpr std::array<std::pair<uint16_t, uint16_t>, DIFFICULTY_COUNT> BOUNTY_KILL_RANGE = { { { 50, 100 }, { 100, 200 }, { 200, 400 }, { 300, 600 } } };
	// Bounty Points per completed bounty task (Tibia wiki)
	constexpr std::array<uint8_t, DIFFICULTY_COUNT> BOUNTY_POINTS = { 3, 7, 16, 27 };
	// Hunting task points per completed weekly kill task (Tibia wiki)
	constexpr std::array<uint32_t, DIFFICULTY_COUNT> WEEKLY_KILL_POINTS = { 25, 50, 100, 110 };
	constexpr uint32_t WEEKLY_DELIVERY_POINTS = 75;
	// Weekly kill task experience caps (no cap for master)
	constexpr std::array<uint64_t, DIFFICULTY_COUNT> WEEKLY_KILL_XP_CAP = { 200000, 800000, 3000000, 0 };
	// Minimum level to start a weekly difficulty (as used by the client)
	constexpr std::array<uint16_t, DIFFICULTY_COUNT> WEEKLY_MIN_LEVEL = { 8, 30, 150, 300 };

	enum ShopOfferType : uint8_t {
		SHOP_ITEM = 0,
		SHOP_MOUNT = 1,
		SHOP_OUTFIT = 2,
		SHOP_ITEM_DOUBLE = 3,
		SHOP_PROMOTION = 4,
	};

	struct ShopOffer {
		ShopOfferType type = SHOP_ITEM;
		std::string title;
		std::string description;
		uint32_t price = 0;
		uint16_t itemId = 0; // item offers
		uint16_t mountId = 0; // server mount id
		uint16_t lookTypeMale = 0; // outfit offers
		uint16_t lookTypeFemale = 0;
		uint8_t addons = 0; // 0 base, 1 addon1, 2 addon2
	};

	struct BountyOffer {
		uint16_t raceId = 0;
		uint16_t total = 0;
		uint16_t current = 0;
		uint32_t xp = 0;
		uint8_t points = 0;
		uint8_t rarity = 0; // 0 normal, 1 silver, 2 gold
		uint8_t state = 0; // 0 available, 1 active, 2 claimable, 3 claimed
	};

	struct PreferredSlot {
		bool unlocked = false;
		uint16_t preferred = 0;
		uint16_t unwanted = 0;
	};

	struct WeeklyKillTask {
		uint16_t raceId = 0;
		uint16_t total = 0;
		uint16_t current = 0;
		bool rewarded = false;
	};

	struct WeeklyDeliveryTask {
		uint16_t itemId = 0;
		uint32_t total = 0;
		bool claimed = false;
	};

	class Catalog {
	public:
		struct DeliveryCandidate {
			uint16_t itemId = 0;
			uint32_t chance = 0;
			uint32_t avgCount = 1;
		};

		static Catalog &getInstance();

		const std::vector<uint16_t> &getPool(uint8_t difficulty);
		const std::vector<DeliveryCandidate> &getDeliveryPool(uint8_t difficulty);
		const std::vector<ShopOffer> &getShop();
		bool isInAnyPool(uint16_t raceId);
		uint32_t getMonsterExperience(uint16_t raceId) const;

	private:
		void ensureLoaded();

		bool m_loaded = false;
		std::array<std::vector<uint16_t>, DIFFICULTY_COUNT> m_pools;
		std::array<std::vector<DeliveryCandidate>, DIFFICULTY_COUNT> m_delivery;
		std::vector<ShopOffer> m_shop;
	};

	uint32_t getTalismanBonusHundredths(uint8_t level, uint8_t path);
	uint8_t getTalismanMaxLevel(uint8_t path);
	uint8_t getTalismanStartLevel(uint8_t path);
	uint64_t getLevelExperienceReward(uint32_t level);
}

class PlayerTaskBoard {
public:
	explicit PlayerTaskBoard(Player &player);

	// Lifecycle
	void load();
	void save();
	void onLogin();

	// Client requests
	void handleAction(uint8_t option, NetworkMessage &msg);

	// Packets
	void sendBountyData();
	void sendWeeklyData();
	void sendShopData();
	void sendResources() const;

	// Gameplay hooks
	void onMonsterKilled(const std::shared_ptr<MonsterType> &mType);
	// Active bounty task creature talisman effects (percent, 0 when raceId is not the active task)
	double getDamagePercent(uint16_t raceId);
	double getLifeLeechPercent(uint16_t raceId);
	double getLootPercent(uint16_t raceId);
	double getDoubleBestiaryPercent(uint16_t raceId);
	bool isActiveBountyRace(uint16_t raceId);
	bool hasBountyTalismanEquipped() const;

	// Accessors / admin
	uint32_t getBountyPoints();
	uint32_t getSoulseals();
	uint32_t getPromotionPoints();
	uint64_t getPointsSpent();
	bool hasWeeklyExpansion();
	void setWeeklyExpansion(bool value);
	void addBountyPoints(uint32_t amount);
	void addSoulseals(uint32_t amount);
	void addRerollTokens(uint8_t amount);

private:
	// Persistent state
	struct State {
		// Bounty
		uint8_t difficulty = 0;
		std::array<TaskBoard::BountyOffer, TaskBoard::BOUNTY_OFFERS> offers {};
		bool offersGenerated = false;
		int8_t activeIndex = -1;
		uint8_t rerollTokens = 0;
		uint32_t bountyPoints = 0;
		int64_t nextDailyClaim = 0;
		std::array<uint8_t, TaskBoard::TALISMAN_PATHS> talisman { 1, 1, 1, 5 };
		std::array<TaskBoard::PreferredSlot, TaskBoard::PREFERRED_SLOTS> slots {};

		// Weekly
		bool weeklyStarted = false; // difficulty selected for the current/last week
		bool weeklyFinished = false; // week ended and rewards paid, waiting for a new difficulty selection
		uint8_t weeklyDifficulty = 0;
		int64_t weeklyReset = 0;
		uint16_t anyTotal = 0;
		uint16_t anyCurrent = 0;
		bool anyRewarded = false;
		std::vector<TaskBoard::WeeklyKillTask> weeklyKills;
		std::vector<TaskBoard::WeeklyDeliveryTask> weeklyDeliveries;
		uint32_t lastPoints = 0;
		uint32_t lastSeals = 0;

		// Shop / currencies
		uint32_t soulseals = 0;
		uint8_t promotionBought = 0;
		bool weeklyExpansion = false;
		std::vector<uint8_t> shopBought; // purchased one-time offers (indices into catalog)
		uint64_t pointsSpent = 0; // lifetime Hunting Task Points spent in the shop (character titles)
		std::array<uint32_t, 3> completedTasks { 0, 0, 0 }; // weekly tasks completed: beginner, adept, expert+master (achievements)
	};

	void ensureLoaded();
	std::string serialize() const;
	bool deserialize(const std::string &data);

	// Bounty
	void generateOffers();
	void changeDifficulty(uint8_t difficulty);
	void selectTask(uint8_t index);
	void claimReward();
	void reroll();
	void claimDaily();
	void upgradeTalisman(uint8_t path);
	void unlockSlot(uint16_t slot);
	void assignPreferred(uint16_t slot, uint16_t raceId, bool unwanted);
	void clearPreferred(uint16_t slot, bool unwanted);
	uint8_t getDailyMode() const;
	uint32_t getTalismanCost(uint8_t path) const;
	TaskBoard::BountyOffer makeOffer(uint8_t difficulty, uint16_t raceId) const;
	uint32_t computeBountyXp(uint16_t raceId, uint16_t total, uint8_t rarity) const;

	// Weekly
	void checkWeeklyRollover();
	void finishWeek();
	void selectWeeklyDifficulty(uint8_t difficulty);
	void deliverItem(uint8_t slot);
	void updateDeliveryProgress();
	void completeKillTask(TaskBoard::WeeklyKillTask &task);
	void onWeeklyTaskCompleted();
	void checkShopAchievements();
	uint32_t countCompletedKillTasks() const;
	uint32_t countCompletedDeliveryTasks() const;
	uint32_t rewardMultiplier(uint32_t completed) const;
	uint32_t projectedPoints() const;
	uint64_t killTaskExperience() const;
	uint8_t killSlotCount() const;
	uint8_t deliverySlotCount() const;
	uint8_t unlockedDifficulty() const;
	uint32_t getDeliveryCurrent(const TaskBoard::WeeklyDeliveryTask &task) const;

	// Shop
	void buyOffer(uint8_t index);
	uint8_t shopStatus(size_t index, const TaskBoard::ShopOffer &offer) const;
	bool isOfferBought(size_t index) const;
	uint32_t promotionCost(uint8_t alreadyBought) const;

	Player &m_player;
	State m_state;
	bool m_loaded = false;
};
