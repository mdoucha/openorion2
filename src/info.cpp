/*
 * This file is part of OpenOrion2
 * Copyright (C) 2024 Martin Doucha
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include <cstring>
#include "lang.h"
#include "screen.h"
#include "info.h"

#define INFO_ARCHIVE "info.lbx"
#define ASSET_INFO_BG 0
#define ASSET_INFO_CURSOR 1
#define ASSET_INFO_RETURN_BUTTON 2
#define ASSET_INFO_HISTORY_BUTTON 3
#define ASSET_INFO_UP_BUTTON_DISABLED 12
#define ASSET_INFO_DOWN_BUTTON_DISABLED 13
#define ASSET_INFO_UP_BUTTON 14
#define ASSET_INFO_DOWN_BUTTON 15
#define ASSET_INFO_ACHIEVEMENTS_BUTTON 16
#define ASSET_INFO_MORE_BUTTON 20
#define ASSET_INFO_VBAR 22
#define ASSET_INFO_VBAR_MASK 23
#define ASSET_INFO_BAR_LABEL 24
#define ASSET_INFO_VBAR_FOOT 25

#define TECHPIC_ARCHIVE "app_pics.lbx"
#define ASSET_TECHPIC_GRID 0

#define INFO_BAR_COUNT 7
#define INFO_BAR_COLORS 8

struct TechnologyReviewGroup {
	int title_id;
	const Technology *techs;
};

static const uint8_t bar_color_maps[INFO_BAR_COUNT][INFO_BAR_COLORS] = {
	{0, 176, 152, 153, 154, 155, 156, 157},
	{0, 176, 13, 15, 17, 19, 20, 22},
	{0, 176, 164, 165, 166, 167, 168, 169},
	{0, 176, 170, 171, 172, 173, 174, 175},
	{0, 176, 158, 159, 160, 161, 162, 163},
	{0, 176, 146, 147, 148, 149, 150, 151},
	{0, 176, 181, 182, 183, 184, 185, 186},
};

static const TechnologyReviewGroup review_groups[REVIEW_GROUP_COUNT][10] = {
	{
		{
			BILL2_TECHGROUP_CONSTRUCTION,
			(const Technology[]){
				TECH_DOOM_STAR_CONSTRUCTION,
				TECH_PLANET_CONSTRUCTION,
				TECH_TITAN_CONSTRUCTION,
				TECH_TRANSPORT,
				TECH_OUTPOST_SHIP,
				TECH_FREIGHTERS,
				TECH_COLONY_SHIP,
				TECH_COLONY_BASE,
				TECH_NONE
			}
		},
		{
			BILL2_TECHGROUP_SPIES,
			(const Technology[]){
				TECH_CYBERSECURITY_LINK,
				TECH_STEALTH_SUIT,
				TECH_PSIONICS,
				TECH_TELEPATHIC_TRAINING,
				TECH_NEURAL_SCANNER,
				TECH_SPY_NETWORK,
				TECH_NONE
			}
		},
		{
			BILL2_TECHGROUP_COLONY,
			(const Technology[]){
				TECH_STAR_GATE,
				TECH_JUMP_GATE,
				TECH_VIRTUAL_REALITY_NETWORK,
				TECH_BIOMORPHIC_FUNGI,
				TECH_ANDROID_WORKERS,
				TECH_ANDROID_SCIENTISTS,
				TECH_ANDROID_FARMERS,
				TECH_ADVANCED_CITY_PLANNING,
				TECH_NANO_DISASSEMBLERS,
				TECH_MICROLITE_CONSTRUCTION,
				TECH_UNIVERSAL_ANTIDOTE,
				TECH_MICROBIOTICS,
				TECH_NONE
			}
		},
		{
			BILL2_TECHGROUP_SHIP_EQUIP,
			(const Technology[]){
				TECH_MEGAFLUXERS,
				TECH_DAUNTLESS_GUIDANCE_SYSTEM,
				TECH_EMISSIONS_GUIDANCE_SYSTEM,
				TECH_ADVANCED_DAMAGE_CONTROL,
				TECH_SURVIVAL_PODS,
				TECH_NONE
			}
		},
		{
			BILL2_TECHGROUP_GROUND_COMBAT,
			(const Technology[]){
				TECH_BATTLEOIDS,
				TECH_POWERED_ARMOR,
				TECH_PERSONAL_SHIELD,
				TECH_ANTIGRAV_HARNESS,
				TECH_PLASMA_RIFLE,
				TECH_PHASOR_RIFLE,
				TECH_FUSION_RIFLE,
				TECH_LASER_RIFLE,
				TECH_PULSE_RIFLE,
				TECH_NONE
			}
		},
		{
			BILL2_TECHGROUP_MISC,
			(const Technology[]){
				TECH_HYPERSPACE_COMMUNICATIONS,
				TECH_SUBSPACE_COMMUNICATIONS,
				TECH_TACHYON_COMMUNICATIONS,
				TECH_IMPERIUM,
				TECH_GALACTIC_UNIFICATION,
				TECH_FEDERATION,
				TECH_CONFEDERATION,
				TECH_XENO_PSYCHOLOGY,
				TECH_HEIGHTENED_INTELLIGENCE,
				TECH_EVOLUTIONARY_MUTATION,
				TECH_CAPITOL,
				TECH_NONE
			}
		},
		{
			-1,
			{}
		},
	},
	{
		{
			BILL2_TECHGROUP_FOOD,
			(const Technology[]){
				TECH_WEATHER_CONTROL_SYSTEM,
				TECH_FOOD_REPLICATORS,
				TECH_SOIL_ENRICHMENT,
				TECH_HYDROPONIC_FARM,
				TECH_GAIA_TRANSFORMATION,
				TECH_SUBTERRANEAN_FARMS,
				TECH_TERRAFORMING,
				TECH_NONE
			}
		},
		{
			BILL2_TECHGROUP_POLLUTION,
			(const Technology[]){
				TECH_CORE_WASTE_DUMPS,
				TECH_ATMOSPHERIC_RENEWER,
				TECH_POLLUTION_PROCESSOR,
				TECH_NONE
			}
		},
		{
			BILL2_TECHGROUP_PRODUCTION,
			(const Technology[]){
				TECH_DEEP_CORE_MINING,
				TECH_ROBOTIC_FACTORY,
				TECH_RECYCLOTRON,
				TECH_ROBOMINERS,
				TECH_AUTOMATED_FACTORIES,
				TECH_NONE
			}
		},
		{
			BILL2_TECHGROUP_RESEARCH,
			(const Technology[]){
				TECH_GALACTIC_CYBERNET,
				TECH_PLANETARY_SUPERCOMPUTER,
				TECH_AUTOLAB,
				TECH_RESEARCH_LABORATORY,
				TECH_NONE
			}
		},
		{
			BILL2_TECHGROUP_MONEY,
			(const Technology[]){
				TECH_GALACTIC_CURRENCY_EXCHANGE,
				TECH_PLANETARY_STOCK_EXCHANGE,
				TECH_SPACEPORT,
				TECH_NONE
			}
		},
		{
			BILL2_TECHGROUP_COLONY_DEFENSE,
			(const Technology[]){
				TECH_ARTEMIS_SYSTEM_NET,
				TECH_PLANETARY_BARRIER_SHIELD,
				TECH_PLANETARY_FLUX_SHIELD,
				TECH_PLANETARY_RADIATION_SHIELD,
				TECH_STAR_FORTRESS,
				TECH_BATTLESTATION,
				TECH_STAR_BASE,
				TECH_GROUND_BATTERIES,
				TECH_PLANETARY_MISSILE_BASE,
				TECH_FIGHTER_GARRISON,
				TECH_ARMOR_BARRACKS,
				TECH_MARINE_BARRACKS,
				TECH_NONE
			}
		},
		{
			BILL2_TECHGROUP_MORALE,
			(const Technology[]){
				TECH_PLEASURE_DOME,
				TECH_HOLO_SIMULATOR,
				TECH_NONE
			}
		},
		{
			BILL2_TECHGROUP_MISC,
			(const Technology[]){
				TECH_BIOSPHERES,
				TECH_WARP_INTERDICTOR,
				TECH_DIMENSIONAL_PORTAL,
				TECH_ALIEN_MANAGEMENT_CENTER,
				TECH_PLANETARY_GRAVITY_GENERATOR,
				TECH_CLONING_CENTER,
				TECH_SPACE_ACADEMY,
				TECH_ASTRO_UNIVERSITY,
				TECH_NONE
			}
		},
		{
			-1,
			{}
		},
	},
	{
		{
			BILL2_TECHGROUP_BEAMS,
			(const Technology[]){
				TECH_STELLAR_CONVERTER,
				TECH_DEATH_RAY,
				TECH_PARTICLE_BEAM,
				TECH_MAULER_DEVICE,
				TECH_DISRUPTER_CANNON,
				TECH_PLASMA_CANNON,
				TECH_GRAVITON_BEAM,
				TECH_PHASOR,
				TECH_ION_PULSE_CANNON,
				TECH_NEUTRON_BLASTER,
				TECH_GAUSS_CANNON,
				TECH_FUSION_BEAM,
				TECH_MASS_DRIVER,
				TECH_LASER_CANNON,
				TECH_NONE
			}
		},
		{
			BILL2_TECHGROUP_MISSILES,
			(const Technology[]){
				TECH_PLASMA_TORPEDOES,
				TECH_PROTON_TORPEDOES,
				TECH_ANTIMATTER_TORPEDOES,
				TECH_ZEON_MISSILE,
				TECH_PULSON_MISSILE,
				TECH_MERCULITE_MISSILE,
				TECH_NUCLEAR_MISSILE,
				TECH_NONE
			}
		},
		{
			BILL2_TECHGROUP_BOMBS,
			(const Technology[]){
				TECH_BIOTERMINATOR,
				TECH_DEATH_SPORES,
				TECH_NEUTRONIUM_BOMB,
				TECH_ANTIMATTER_BOMB,
				TECH_FUSION_BOMB,
				TECH_NUCLEAR_BOMB,
				TECH_NONE
			}
		},
		{
			BILL2_TECHGROUP_FIGHTERS,
			(const Technology[]){
				TECH_HEAVY_FIGHTER_BAYS,
				TECH_BOMBER_BAYS,
				TECH_FIGHTER_BAYS,
				TECH_ASSAULT_SHUTTLES,
				TECH_NONE
			}
		},
		{
			BILL2_TECHGROUP_SPECIAL,
			(const Technology[]){
				TECH_SPATIAL_COMPRESSOR,
				TECH_BLACK_HOLE_GENERATOR,
				TECH_STASIS_FIELD,
				TECH_PULSAR,
				TECH_PLASMA_WEB,
				TECH_GYRO_DESTABILIZER,
				TECH_ANTIMISSILE_ROCKETS,
				TECH_NONE
			}
		},
		{
			-1,
			{}
		},
	},
	{
		{
			BILL2_TECHGROUP_SHIELDS,
			(const Technology[]){
				TECH_HARD_SHIELDS,
				TECH_MULTIPHASED_SHIELDS,
				TECH_SHIELD_CAPACITORS,
				TECH_CLASS_X_SHIELD,
				TECH_CLASS_VII_SHIELD,
				TECH_CLASS_V_SHIELD,
				TECH_CLASS_III_SHIELD,
				TECH_CLASS_I_SHIELD,
				TECH_NONE
			}
		},
		{
			BILL2_TECHGROUP_ARMOR,
			(const Technology[]){
				TECH_REINFORCED_HULL,
				TECH_HEAVY_ARMOR,
				TECH_XENTRONIUM_ARMOR,
				TECH_ADAMANTIUM_ARMOR,
				TECH_NEUTRONIUM_ARMOR,
				TECH_ZORTRIUM_ARMOR,
				TECH_TRITANIUM_ARMOR,
				TECH_TITANIUM_ARMOR,
				TECH_NONE
			}
		},
		{
			BILL2_TECHGROUP_DRIVES,
			(const Technology[]){
				TECH_AUGMENTED_ENGINES,
				TECH_INTERPHASED_DRIVE,
				TECH_HYPER_DRIVE,
				TECH_ANTIMATTER_DRIVE,
				TECH_ION_DRIVE,
				TECH_FUSION_DRIVE,
				TECH_NUCLEAR_DRIVE,
				TECH_NONE
			}
		},
		{
			BILL2_TECHGROUP_FUELS,
			(const Technology[]){
				TECH_EXTENDED_FUEL_TANKS,
				TECH_THORIUM_FUEL_CELLS,
				TECH_URRIDIUM_FUEL_CELLS,
				TECH_IRIDIUM_FUEL_CELLS,
				TECH_DEUTERIUM_FUEL_CELLS,
				TECH_STANDARD_FUEL_CELLS,
				TECH_NONE
			}
		},
		{
			BILL2_TECHGROUP_COMPUTERS,
			(const Technology[]){
				TECH_MOLECULARTRONIC_COMPUTER,
				TECH_CYBERTRONIC_COMPUTER,
				TECH_POSITRONIC_COMPUTER,
				TECH_OPTRONIC_COMPUTER,
				TECH_ELECTRONIC_COMPUTER,
				TECH_NONE
			}
		},
		{
			BILL2_TECHGROUP_SCANNERS,
			(const Technology[]){
				TECH_SENSORS,
				TECH_NEUTRON_SCANNER,
				TECH_TACHYON_SCANNER,
				TECH_SPACE_SCANNER,
				TECH_NONE
			}
		},
		{
			BILL2_TECHGROUP_SHIP_DEFENSE,
			(const Technology[]){
				TECH_DAMPER_FIELD,
				TECH_REFLECTION_FIELD,
				TECH_ENERGY_ABSORBER,
				TECH_DISPLACEMENT_DEVICE,
				TECH_LIGHTNING_FIELD,
				TECH_PHASING_CLOAK,
				TECH_CLOAKING_DEVICE,
				TECH_INERTIAL_NULLIFIER,
				TECH_INERTIAL_STABILIZER,
				TECH_WIDE_AREA_JAMMER,
				TECH_MULTIWAVE_ECM_JAMMER,
				TECH_ECM_JAMMER,
				TECH_NONE
			}
		},
		{
			BILL2_TECHGROUP_OFFENSE,
			(const Technology[]){
				TECH_ACHILLES_TARGETING_UNIT,
				TECH_STRUCTURAL_ANALYZER,
				TECH_HIGH_ENERGY_FOCUS,
				TECH_RANGEMASTER_UNIT,
				TECH_BATTLE_SCANNER,
				TECH_NONE
			}
		},
		{
			BILL2_TECHGROUP_MISC,
			(const Technology[]){
				TECH_QUANTUM_DETONATOR,
				TECH_PHASE_SHIFTER,
				TECH_TIME_WARP_FACILITATOR,
				TECH_HYPERX_CAPACITORS,
				TECH_SUBSPACE_TELEPORTER,
				TECH_TRANSPORTERS,
				TECH_TRACTOR_BEAM,
				TECH_AUTOMATED_REPAIR_UNIT,
				TECH_STEALTH_FIELD,
				TECH_SECURITY_STATIONS,
				TECH_FAST_MISSILE_RACKS,
				TECH_WARP_DISSIPATER,
				TECH_SCOUT_LAB,
				TECH_BATTLE_PODS,
				TECH_TROOP_PODS,
				TECH_NONE
			}
		},
		{
			-1,
			{}
		},
	},
};

static void drawInfoBox(int x, int y, unsigned width, unsigned height) {
	unsigned ypos;

	if (width < 7 || height < 7) {
		return;
	}

	gameScreen->fillRect(x + 2, y, 1, 1, RGB(0x084408));
	gameScreen->fillRect(x + 3, y, width - 6, 1, RGB(0x083808));
	gameScreen->fillRect(x + width - 3, y, 1, 1, RGB(0x082808));
	gameScreen->fillRect(x + 2, y + 1, 1, 2, RGB(0x082808));
	gameScreen->fillRect(x + 3, y + 1, width - 6, 2, RGB(0x081c08));
	gameScreen->fillRect(x + width - 3, y + 1, 1, 2, RGB(0x080c08));

	ypos = y + 3;
	gameScreen->fillRect(x, ypos, 1, height - 6, RGB(0x082808));
	gameScreen->fillRect(x + 1, ypos, width - 2, height - 6, RGB(0x081c08));
	gameScreen->fillRect(x + width - 1, ypos, 1, height - 6, RGB(0x080c08));

	for (; ypos < y + height - 3; ypos += 3) {
		gameScreen->fillRect(x, ypos, 1, 1, RGB(0x084408));
		gameScreen->fillRect(x + 1, ypos, width - 2, 1, RGB(0x083808));
		gameScreen->fillRect(x + width - 1, ypos, 1, 1, RGB(0x082808));
	}

	gameScreen->fillRect(x + 2, y + height - 3, 1, 1, RGB(0x084408));
	gameScreen->fillRect(x + 3, y + height - 3, width - 6, 1,
		RGB(0x083808));
	gameScreen->fillRect(x + width - 3, y + height - 3, 1, 1,
		RGB(0x082808));
	gameScreen->fillRect(x + 2, y + height - 2, 1, 2, RGB(0x082808));
	gameScreen->fillRect(x + 3, y + height - 2, width - 6, 2,
		RGB(0x081c08));
	gameScreen->fillRect(x + width - 3, y + height - 2, 1, 2,
		RGB(0x080c08));
}

HistoryGraphWidget::HistoryGraphWidget(unsigned x, unsigned y, unsigned width,
	unsigned height, const GameState *game, int activePlayer) :
	Widget(x, y, width, height), _game(game), _activePlayer(activePlayer) {

}

HistoryGraphWidget::~HistoryGraphWidget(void) {

}

void HistoryGraphWidget::redraw(int x, int y, unsigned curtick) {
	Font *titleFnt;
	const char *str;

	if (isHidden()) {
		return;
	}

	x += getX();
	y += getY();
	titleFnt = gameFonts->getFont(FONTSIZE_TITLE);

	str = gameLang->misctext(TXT_MISC_BILLTEXT, BILL_INFO_TITLE_HISTORY);
	titleFnt->centerText(x + 208, y + 31, TITLE_COLOR_INFO, str,
		OUTLINE_NONE, 3);

	drawInfoBox(x + 14, y + 60, 390, 63);
	drawInfoBox(x + 14, y + 132, 390, 282);
}

TechReviewWidget::TechReviewWidget(unsigned x, unsigned y, unsigned width,
	unsigned height, const GameState *game, int activePlayer,
	int targetPlayer) : CompositeWidget(x, y, width, height), _game(game),
	_groupChoice(NULL), _upButton(NULL), _downButton(NULL),
	_techImage(NULL), _description(NULL), _activePlayer(activePlayer),
	_targetPlayer(targetPlayer) {

	ImageAsset cursor;

	if (activePlayer < 0 || activePlayer >= _game->_playerCount) {
		throw std::out_of_range("Invalid active player");
	}

	if (targetPlayer < 0 || targetPlayer >= _game->_playerCount) {
		throw std::out_of_range("Invalid target player");
	}

	cursor = gameAssets->getImage(INFO_ARCHIVE, ASSET_INFO_CURSOR);
	_gridImage = gameAssets->getImage(TECHPIC_ARCHIVE, ASSET_TECHPIC_GRID,
		cursor->palette());
	initWidgets();
}

TechReviewWidget::~TechReviewWidget(void) {
	delete _techImage;
	delete _description;
}

void TechReviewWidget::initWidgets(void) {
	int i, j, x;
	unsigned color, count;
	ImageAsset asset, cursor;
	Widget *w;
	const uint8_t *pal;
	const char *str;
	const Player *aplayer, *tplayer;
	const Technology *techs;
	TechListWidget::TechListItem items[MAX_APPLIED_TECHS];

	aplayer = _game->_players + _activePlayer;
	tplayer = _game->_players + _targetPlayer;
	cursor = gameAssets->getImage(INFO_ARCHIVE, ASSET_INFO_CURSOR);
	pal = cursor->palette();

	w = createWidget(204, 65, INFO_ARCHIVE, ASSET_INFO_UP_BUTTON, pal, 1);
	w->setIdleSprite(INFO_ARCHIVE, ASSET_INFO_UP_BUTTON, pal, 0);
	w->setDisabledSprite(INFO_ARCHIVE, ASSET_INFO_UP_BUTTON_DISABLED, pal,
		0);
	w->setMouseUpCallback(MBUTTON_LEFT, GuiMethod<TechReviewWidget>(*this,
		&TechReviewWidget::previousListPage));
	_upButton = w;

	w = createWidget(204, 395, INFO_ARCHIVE, ASSET_INFO_DOWN_BUTTON, pal,
		1);
	w->setIdleSprite(INFO_ARCHIVE, ASSET_INFO_DOWN_BUTTON, pal, 0);
	w->setDisabledSprite(INFO_ARCHIVE, ASSET_INFO_DOWN_BUTTON_DISABLED,
		pal, 0);
	w->setMouseUpCallback(MBUTTON_LEFT, GuiMethod<TechReviewWidget>(*this,
		&TechReviewWidget::nextListPage));
	_downButton = w;

	_groupChoice = new ChoiceWidget(10, 427, 300, 18, REVIEW_GROUP_COUNT);
	addWidget(_groupChoice);
	_groupChoice->setValueChangeCallback(GuiMethod<TechReviewWidget>(*this,
		&TechReviewWidget::changeReviewGroup));

	for (i = 0, x = 0; i < REVIEW_GROUP_COUNT; i++) {
		asset = gameAssets->getImage(INFO_ARCHIVE,
			ASSET_INFO_ACHIEVEMENTS_BUTTON + i, pal);
		_groupChoice->setChoiceButton(i, x, 0, asset->width(), 18,
			(Image*)asset, 1);
		_groupChoice->button(i)->setIdleSprite((Image*)asset, 0);
		x += asset->width();

		_reviewGroups[i] = new TechListWidget(15, 62, 203, 350,
			FONTSIZE_MEDIUM, FONTSIZE_SMALL);
		addWidget(_reviewGroups[i]);
		_reviewGroups[i]->setItemHighlightCallback(
			GuiMethod<TechReviewWidget>(*this,
			&TechReviewWidget::changeHighlightedTech));

		for (j = 0; review_groups[i][j].title_id >= 0; j++) {
			techs = review_groups[i][j].techs;

			for (count = 0; *techs != TECH_NONE; techs++) {
				if (!tplayer->knowsTechnology(*techs)) {
					continue;
				}

				color = FONT_COLOR_INFO_NORMAL;

				if (!aplayer->knowsTechnology(*techs)) {
					color = FONT_COLOR_TECH_DIFF;
				}

				items[count].tech_id = *techs;
				items[count].color = color;
				items[count].name = copystr(gameLang->techname(
					TNAME_TECH_NONE + *techs));
				count++;
			}

			if (!count) {
				continue;
			}

			str = gameLang->misctext(TXT_MISC_BILLTEX2,
				review_groups[i][j].title_id);
			_reviewGroups[i]->addGroup(str, FONT_COLOR_INFO_NORMAL,
				items, count);

			while (count) {
				delete[] items[--count].name;
			}
		}

		_reviewGroups[i]->highlightFirstItem();
	}

	changeReviewGroup(0, 0, 0);
}

void TechReviewWidget::changeReviewGroup(int x, int y, int arg) {
	unsigned i;
	TechListWidget *w;

	for (i = 0; i < REVIEW_GROUP_COUNT; i++) {
		_reviewGroups[i]->hide(i != _groupChoice->value());
	}

	w = _reviewGroups[_groupChoice->value()];
	_upButton->disable(!w->currentPage());
	_downButton->disable(w->currentPage() + 1 >= w->pageCount());
	changeHighlightedTech(x, y, arg);
}

void TechReviewWidget::changeHighlightedTech(int x, int y, int arg) {
	TechListWidget *w = _reviewGroups[_groupChoice->value()];
	GuiSprite *newpic = NULL, *oldpic = _techImage;
	TextLayout *newdesc = NULL, *olddesc = _description;
	ImageAsset cursor;
	Technology tech_id;
	const HelpText *entry;
	int item = w->highlightedItem();

	if (item < 0) {
		return;
	}

	w->selectItem(w->highlightedGroup(), item);
	tech_id = w->highlightedTechID();

	if (tech_id != TECH_NONE) {
		cursor = gameAssets->getImage(INFO_ARCHIVE, ASSET_INFO_CURSOR);
		entry = gameLang->help(tech_id);
		newdesc = new TextLayout;

		try {
			newdesc->setFont(FONTSIZE_BIG,
				FONT_COLOR_INFO_NORMAL, 2, OUTLINE_NONE, 2);
			newdesc->appendText(entry->title, 0, 0, 173,
				ALIGN_CENTER);
			newdesc->setFont(FONTSIZE_SMALL,
				FONT_COLOR_INFO_NORMAL, 1, OUTLINE_NONE, 2);
			newdesc->appendText(entry->text, 0, 196, 173);

			newpic = new GuiSprite(TECHPIC_ARCHIVE, tech_id,
				cursor->palette(), 0, 0, 0);
		} catch (...) {
			delete newdesc;
			delete newpic;
			throw;
		}

		_description = newdesc;
		_techImage = newpic;
	} else {
		_techImage = NULL;
		_description = NULL;
	}

	if (oldpic) {
		oldpic->discard();
	}

	if (olddesc) {
		olddesc->discard();
	}
}

void TechReviewWidget::previousListPage(int x, int y, int arg) {
	TechListWidget *w = _reviewGroups[_groupChoice->value()];

	w->previousPage();
	w->highlightFirstItem();
	_upButton->disable(!w->currentPage());
	_downButton->disable(w->currentPage() + 1 >= w->pageCount());
	changeHighlightedTech(x, y, arg);
}

void TechReviewWidget::nextListPage(int x, int y, int arg) {
	TechListWidget *w = _reviewGroups[_groupChoice->value()];

	w->nextPage();
	w->highlightFirstItem();
	_upButton->disable(!w->currentPage());
	_downButton->disable(w->currentPage() + 1 >= w->pageCount());
	changeHighlightedTech(x, y, arg);
}

void TechReviewWidget::redraw(int x, int y, unsigned curtick) {
	Font *titleFnt;
	const char *str;
	GuiSprite *techpic = _techImage;
	TextLayout *techdesc = _description;

	if (isHidden()) {
		return;
	}

	x += getX();
	y += getY();
	titleFnt = gameFonts->getFont(FONTSIZE_TITLE);

	str = gameLang->misctext(TXT_MISC_BILLTEXT, BILL_INFO_TITLE_TECH);
	titleFnt->centerText(x + 208, y + 31, TITLE_COLOR_INFO, str,
		OUTLINE_NONE, 3);

	drawInfoBox(x + 12, y + 60, 208, 354);
	drawInfoBox(x + 227, y + 60, 180, 45);
	drawInfoBox(x + 227, y + 264, 180, 150);

	gameScreen->fillRect(x + 227, y + 115, _gridImage->width(),
		_gridImage->height(), 0, 0, 0);
	_gridImage->draw(x + 227, y + 115);

	if (techpic) {
		techpic->redraw(x + 227, y + 115, curtick);
	}

	if (techdesc) {
		techdesc->redraw(x + 230, y + 72, curtick);
	}

	redrawWidgets(x, y, curtick);
}

RaceInfoWidget::RaceInfoWidget(unsigned x, unsigned y, unsigned width,
	unsigned height, const GameState *game, int activePlayer) :
	CompositeWidget(x, y, width, height), _game(game),
	_activePlayer(activePlayer), _page(0) {

	initWidgets();
}

RaceInfoWidget::~RaceInfoWidget(void) {

}

void RaceInfoWidget::initWidgets(void) {
	if (playerCount() > 4) {
		ImageAsset cursor;
		Widget *w;

		cursor = gameAssets->getImage(INFO_ARCHIVE, ASSET_INFO_CURSOR);
		w = createWidget(179, 427, INFO_ARCHIVE, ASSET_INFO_MORE_BUTTON,
			cursor->palette(), 1);
		w->setIdleSprite(INFO_ARCHIVE, ASSET_INFO_MORE_BUTTON,
			cursor->palette(), 0);
		w->setMouseUpCallback(MBUTTON_LEFT, GuiMethod(*this,
			&RaceInfoWidget::nextPage));
	}
}

unsigned RaceInfoWidget::playerCount(void) const {
	unsigned i, ret;

	for (i = 0, ret = 0; i < _game->_playerCount; i++) {
		if (_activePlayer == (int)i || _game->_players[i].eliminated ||
			_game->_players[_activePlayer].isPlayerVisible(i)) {
			ret++;
		}
	}

	return ret;
}

void RaceInfoWidget::nextPage(int x, int y, int arg) {
	unsigned pagecount = (playerCount() + 3) / 4;

	_page = pagecount ? (_page + 1) % pagecount : 0;
}

void RaceInfoWidget::redraw(int x, int y, unsigned curtick) {
	unsigned i, j, pcount, color;
	int xpos, ypos;
	Font *titleFnt, *raceFnt, *itemFnt;
	const char *str, *fmt;
	const Player *pptr, *players[MAX_PLAYERS] = { NULL };
	StringBuffer buf;

	if (isHidden()) {
		return;
	}

	players[0] = _game->_players + _activePlayer;

	for (i = 0, pcount = 1; i < _game->_playerCount; i++) {
		if (_activePlayer == (int)i || _game->_players[i].eliminated ||
			!players[0]->isPlayerVisible(i)) {
			continue;
		}

		players[pcount++] = _game->_players + i;
	}

	// Eliminated players are listed last regardless of visibility
	for (i = 0; i < _game->_playerCount; i++) {
		if (_game->_players[i].eliminated) {
			players[pcount++] = _game->_players + i;
		}
	}

	x += getX();
	y += getY();
	titleFnt = gameFonts->getFont(FONTSIZE_TITLE);
	raceFnt = gameFonts->getFont(FONTSIZE_BIG);
	itemFnt = gameFonts->getFont(FONTSIZE_SMALL);

	str = gameLang->misctext(TXT_MISC_BILLTEXT, BILL_INFO_TITLE_RACE);
	titleFnt->centerText(x + 208, y + 31, TITLE_COLOR_INFO, str,
		OUTLINE_NONE, 3);

	for (i = 0; i < 4; i++) {
		xpos = x + (i % 2 ? 213 : 17);
		ypos = y + (i / 2 ? 243 : 60);
		drawInfoBox(xpos, ypos, 188, 171);
		gameScreen->fillRect(xpos + 13, ypos + 21, 164, 1,
			RGB(0x0c840c));

		pptr = players[i + 4 * _page];

		if (!pptr) {
			str = gameLang->misctext(TXT_MISC_BILLTEXT,
				BILL_INFO_NO_CONTACT);
			raceFnt->centerText(xpos + 93, ypos + 4,
				FONT_COLOR_INFO_NORMAL, str, OUTLINE_NONE, 2);
			continue;
		}

		buf = pptr->race;
		buf.toUpper();
		color = FONT_COLOR_INFO_RED + pptr->color;
		raceFnt->centerText(xpos + 93, ypos + 4, color, buf.c_str(),
			OUTLINE_NONE, 2);

		if (pptr->eliminated) {
			str = gameLang->misctext(TXT_MISC_BILLTEXT,
				BILL_INFO_ELIMINATED);
			itemFnt->centerText(xpos + 93, ypos + 17,
				FONT_COLOR_INFO_ELIMINATED, str, OUTLINE_FULL,
				2);
		}

		ypos += 26;
		str = gameLang->estrings(ESTR_GOVERNMENT_NAMES +
			pptr->traits[TRAIT_GOVERNMENT]);
		buf.printf("^ %s", str);
		itemFnt->renderText(xpos + 3, ypos, color, buf.c_str(),
			OUTLINE_NONE, 2);
		ypos += itemFnt->height() + 1;

		for (j = 1; j < TRAITS_COUNT; j++) {
			if (!pptr->traits[j] || (j == TRAIT_RICH_HOMEWORLD &&
				pptr->traits[j] < 0)) {
				continue;
			}

			str = gameLang->raceInfo(j);
			buf.printf("^ %s", str);

			if (j == TRAIT_FARMING || j == TRAIT_MONEY) {
				if (pptr->traits[j] % 2) {
					fmt = "%+3.1f";
				} else {
					fmt = "%+2.0f";
				}

				buf.append_printf(fmt, pptr->traits[j] / 2.0f);

				if (j == TRAIT_MONEY) {
					buf.append(" BC");
				}
			} else if (j < TRAIT_LOW_G) {
				buf.append_printf("%+d", pptr->traits[j]);
			}

			itemFnt->renderText(xpos + 3, ypos, color, buf.c_str(),
				OUTLINE_NONE, 2);
			ypos += itemFnt->height() + 1;
		}

		if (pptr->traits[TRAIT_RICH_HOMEWORLD] < 0) {
			str = gameLang->raceInfo(TRAIT_POOR_HOMEWORLD);
			buf.printf("^ %s", str);
			itemFnt->renderText(xpos + 3, ypos, color, buf.c_str(),
				OUTLINE_NONE, 2);
		}
	}

	redrawWidgets(x, y, curtick);
}

TurnSummaryWidget::TurnSummaryWidget(unsigned x, unsigned y, unsigned width,
	unsigned height, const GameState *game, int activePlayer) :
	Widget(x, y, width, height), _game(game), _activePlayer(activePlayer) {

}

TurnSummaryWidget::~TurnSummaryWidget(void) {

}

void TurnSummaryWidget::redraw(int x, int y, unsigned curtick) {
	Font *titleFnt;
	const char *str;

	if (isHidden()) {
		return;
	}

	x += getX();
	y += getY();
	titleFnt = gameFonts->getFont(FONTSIZE_TITLE);

	str = gameLang->misctext(TXT_MISC_BILLTEXT,
		BILL_INFO_TITLE_TURN_SUMMARY);
	titleFnt->centerText(x + 208, y + 31, TITLE_COLOR_INFO, str,
		OUTLINE_NONE, 3);

	drawInfoBox(x + 12, y + 60, 395, 355);
}

DocsWidget::DocsWidget(unsigned x, unsigned y, unsigned width,
	unsigned height) : Widget(x, y, width, height) {

}

DocsWidget::~DocsWidget(void) {

}

void DocsWidget::redraw(int x, int y, unsigned curtick) {
	Font *titleFnt;
	const char *str;

	if (isHidden()) {
		return;
	}

	x += getX();
	y += getY();
	titleFnt = gameFonts->getFont(FONTSIZE_TITLE);

	str = gameLang->misctext(TXT_MISC_BILLTEXT, BILL_INFO_TITLE_REFERENCE);
	titleFnt->centerText(x + 208, y + 31, TITLE_COLOR_INFO, str,
		OUTLINE_NONE, 3);

	drawInfoBox(x + 12, y + 60, 197, 27);
	drawInfoBox(x + 12, y + 93, 197, 321);
	drawInfoBox(x + 212, y + 60, 196, 27);
	drawInfoBox(x + 212, y + 93, 196, 321);
}

InfoView::InfoView(GameState *game, int activePlayer) : _game(game),
	_activePlayer(activePlayer), _panelChoice(NULL) {

	ImageAsset cursor;
	const uint8_t *pal;

	cursor = gameAssets->getImage(INFO_ARCHIVE, ASSET_INFO_CURSOR);
	pal = cursor->palette();
	_bg = gameAssets->getImage(INFO_ARCHIVE, ASSET_INFO_BG, pal);
	_vbar = gameAssets->getBitmap(INFO_ARCHIVE, ASSET_INFO_VBAR);
	_barMask = gameAssets->getBitmap(INFO_ARCHIVE, ASSET_INFO_VBAR_MASK);
	_barLabel = gameAssets->getBitmap(INFO_ARCHIVE, ASSET_INFO_BAR_LABEL);
	_barFoot = gameAssets->getBitmap(INFO_ARCHIVE, ASSET_INFO_VBAR_FOOT);

	initWidgets();
}

InfoView::~InfoView(void) {

}

void InfoView::initWidgets(void) {
	unsigned i, y;
	ImageAsset asset;
	Widget *w;
	const uint8_t *pal = _bg->palette();

	w = createWidget(535, 434, INFO_ARCHIVE, ASSET_INFO_RETURN_BUTTON,
		pal, 1);
	w->setMouseUpCallback(MBUTTON_LEFT, GuiMethod<InfoView>(*this,
		&InfoView::clickReturn));

	_panelChoice = new ChoiceWidget(21, 50, 164, 131, INFO_PANEL_COUNT);
	addWidget(_panelChoice);

	for (i = 0, y = 0; i < INFO_PANEL_COUNT; i++) {
		asset = gameAssets->getImage(INFO_ARCHIVE,
			ASSET_INFO_HISTORY_BUTTON + i, pal);
		_panelChoice->setChoiceButton(i, 0, y, 164, asset->height(),
			(Image*)asset, 1);
		_panelChoice->button(i)->setIdleSprite((Image*)asset, 0);
		y += asset->height();
	}

	_panelChoice->setValueChangeCallback(GuiMethod(*this,
		&InfoView::changePanel));
	i = _game->_players[_activePlayer].infoPanel >> 4;
	_panelChoice->setValue(i < INFO_PANEL_COUNT ? i : 0);

	_panels[0] = new HistoryGraphWidget(206, 0, SCREEN_WIDTH - 206,
		SCREEN_HEIGHT, _game, _activePlayer);
	addWidget(_panels[0]);
	_panels[1] = new TechReviewWidget(206, 0, SCREEN_WIDTH - 206,
		SCREEN_HEIGHT, _game, _activePlayer, _activePlayer);
	addWidget(_panels[1]);
	_panels[2] = new RaceInfoWidget(206, 0, SCREEN_WIDTH - 206,
		SCREEN_HEIGHT, _game, _activePlayer);
	addWidget(_panels[2]);
	_panels[3] = new TurnSummaryWidget(206, 0, SCREEN_WIDTH - 206,
		SCREEN_HEIGHT, _game, _activePlayer);
	addWidget(_panels[3]);
	_panels[4] = new DocsWidget(206, 0, SCREEN_WIDTH - 206, SCREEN_HEIGHT);
	addWidget(_panels[4]);

	changePanel(0, 0, 0);
}

void InfoView::changePanel(int x, int y, int arg) {
	unsigned i;

	for (i = 0; i < INFO_PANEL_COUNT; i++) {
		_panels[i]->hide(i != _panelChoice->value());
	}

	// TODO: Change Player::infoPanel in gamestate
}

void InfoView::redraw(unsigned curtick) {
	unsigned i, x, y, count, date, maxcost = 0, maintcosts[INFO_BAR_COUNT];
	Font *dateFnt, *smallFnt, *labelFnt;
	const Player *pptr = _game->_players + _activePlayer;
	const char *str;
	const uint8_t *pal = _bg->palette();
	StringBuffer buf;
	uint8_t barpal[PALSIZE];

	maintcosts[0] = pptr->bcProduced;
	maintcosts[1] = pptr->buildingMaintenance;
	maintcosts[2] = pptr->freighterMaintenance;
	maintcosts[3] = pptr->shipMaintenance;
	maintcosts[4] = pptr->spyMaintenance;
	maintcosts[5] = pptr->tributeCost;
	maintcosts[6] = pptr->officerMaintenance;
	memcpy(barpal, pal, PALSIZE * sizeof(uint8_t));

	for (i = 0; i < INFO_BAR_COUNT; i++) {
		maxcost = MAX(maxcost, maintcosts[i]);
	}

	for (i = 0; i < INFO_BAR_COUNT; i++) {
		maintcosts[i] = (100 * maintcosts[i] + maxcost / 2) / maxcost;
	}

	dateFnt = gameFonts->getFont(FONTSIZE_BIG);
	smallFnt = gameFonts->getFont(FONTSIZE_SMALLER);
	labelFnt = gameFonts->getFont(FONTSIZE_SMALL);

	gameScreen->fillRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, RGB(0x000000));
	gameScreen->fillRect(212, 23, 408, 434, RGB(0x083808));

	for (y = 23; y < 457; y += 3) {
		gameScreen->fillRect(212, y, 408, 1, RGB(0x082808));
	}

	_bg->draw(0, 0);
	date = _game->_gameConfig.stardate;
	buf.printf("%u.%u", date / 10, date % 10);
	dateFnt->centerText(150, 27, FONT_COLOR_INFO_NORMAL, buf.c_str(),
		OUTLINE_NONE, 2);

	str = gameLang->misctext(TXT_MISC_BILLTEXT, BILL_INFO_INCOME_BAR);
	smallFnt->renderText(27, 324, FONT_COLOR_INFO_NORMAL, str);
	str = gameLang->misctext(TXT_MISC_BILLTEXT, BILL_INFO_MAINTENANCE);
	smallFnt->renderText(91, 324, FONT_COLOR_INFO_NORMAL, str);

	y = 344;
	remapColors(barpal, pal, bar_color_maps[0], INFO_BAR_COLORS);
	_barLabel->draw(25, y, barpal);
	str = gameLang->misctext(TXT_MISC_BILLTEXT, BILL_INFO_INCOME_LABEL);
	buf.printf("%u%s", pptr->bcProduced, str);
	labelFnt->centerText(116, y + 3, FONT_COLOR_INFO_BLACK, buf.c_str(),
		OUTLINE_NONE, 2);
	y += _barLabel->height() - 2;

	for (i = 1, count = 0; i < INFO_BAR_COUNT; i++) {
		remapColors(barpal, pal, bar_color_maps[i], INFO_BAR_COLORS);
		_barLabel->draw(25, y, barpal);
		buf.printf("%u%%", maintcosts[i]);
		labelFnt->centerText(65, y + 3, FONT_COLOR_INFO_BLACK,
			buf.c_str(), OUTLINE_NONE, 2);
		str = gameLang->misctext(TXT_MISC_BILLTEXT,
			BILL_INFO_INCOME_LABEL + i);
		labelFnt->centerText(132, y + 3, FONT_COLOR_INFO_BLACK, str,
			OUTLINE_NONE, 2);
		y += _barLabel->height() - 2;

		if (maintcosts[i]) {
			count++;
		}
	}

	remapColors(barpal, pal, bar_color_maps[0], INFO_BAR_COLORS);
	_vbar->drawTile(30, 212 + 100 - maintcosts[0], 0, 0, _vbar->width(),
		maintcosts[0], barpal);
	_vbar->drawTileMasked(30, 311, 0, maintcosts[0] - 1, _vbar->width(),
		_barMask->height(), barpal, (const Bitmap*)_barMask);
	_barFoot->draw(30, 314, barpal);
	x = 123 - _vbar->width() + (count * (_vbar->width() - 6)) / 2;

	for (i = INFO_BAR_COUNT - 1; i; i--) {
		if (!maintcosts[i]) {
			continue;
		}

		remapColors(barpal, pal, bar_color_maps[i], INFO_BAR_COLORS);
		_vbar->drawTile(x, 212 + 100 - maintcosts[i], 0, 0,
			_vbar->width(), maintcosts[i], barpal);
		_vbar->drawTileMasked(x, 311, 0, maintcosts[i] - 1,
			_vbar->width(), _barMask->height(), barpal,
			(const Bitmap*)_barMask);
		_barFoot->draw(x, 314, barpal);
		x -= _vbar->width() - 6;
	}

	redrawWidgets(0, 0, curtick);
}

void InfoView::clickReturn(int x, int y, int arg) {
	exitView();
}
