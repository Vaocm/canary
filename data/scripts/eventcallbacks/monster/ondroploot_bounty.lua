local callback = EventCallback("MonsterOnDropLootBounty")

-- Bounty Task "More Loot" talisman: chance for an extra loot roll, only vs. the active bounty creature.
function callback.monsterOnDropLoot(monster, corpse)
	local player = Player(corpse:getCorpseOwner())
	if not player or not player:canReceiveLoot() then
		return
	end
	local mType = monster:getType()
	if not mType then
		return
	end

	local chance = player:getBountyLootPercentage(mType:raceId())
	if chance <= 0 or math.random() * 100 >= chance then
		return
	end

	local existingSuffix = corpse:getAttribute(ITEM_ATTRIBUTE_LOOTMESSAGE_SUFFIX) or ""
	local msgSuffix = string.len(existingSuffix) > 0 and ", bounty talisman bonus" or "bounty talisman bonus"

	corpse:addLoot(mType:generateLootRoll({ factor = 1.0, gut = false }, {}, player))
	corpse:setAttribute(ITEM_ATTRIBUTE_LOOTMESSAGE_SUFFIX, existingSuffix .. msgSuffix)
end

callback:register()
