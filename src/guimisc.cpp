/*
 * This file is part of OpenOrion2
 * Copyright (C) 2021 Martin Doucha
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
#include "screen.h"
#include "lang.h"
#include "guimisc.h"

#define TEXTBOX_ARCHIVE "textbox.lbx"
#define ASSET_TEXTBOX_HEADER 0
#define ASSET_TEXTBOX_BODY 1
#define ASSET_TEXTBOX_FOOTER 2
#define ASSET_TEXTBOX_BUTTON 3

#define CONFIRMATION_ARCHIVE "confirm.lbx"
#define ASSET_CONFIRM_BACKGROUND 0
#define ASSET_CONFIRM_YES_BUTTON 1
#define ASSET_CONFIRM_NO_BUTTON 2

#define CONFIRM_TEXT_X 44
#define CONFIRM_TEXT_Y 33
#define CONFIRM_TEXT_WIDTH 223
#define CONFIRM_TEXT_HEIGHT 133

#define ERROR_ARCHIVE "warning.lbx"
#define ASSET_ERROR_BACKGROUND 0

MessageBoxWindow::MessageBoxWindow(GuiView *parent, const char *text,
	unsigned flags) : GuiWindow(parent, flags) {

	initAssets();
	_text.setFont(FONTSIZE_MEDIUM, FONT_COLOR_DEFAULT);
	_text.appendText(text, 0, 19, _width - 40);
	_height += _text.height() >= 20 ? _text.height() - 20 : 0;
	_y = (SCREEN_HEIGHT - _height) / 2;

	initWidgets();
}

MessageBoxWindow::MessageBoxWindow(GuiView *parent, const char *title,
	const char *text, unsigned flags) : GuiWindow(parent, flags) {

	initAssets();
	_text.setFont(FONTSIZE_TITLE, TITLE_COLOR_HELP, 2, OUTLINE_NONE, 2);
	_text.appendText(title, 0, 0, _width - 40, ALIGN_CENTER);
	_text.setFont(FONTSIZE_MEDIUM, FONT_COLOR_HELP, 2, OUTLINE_NONE, 2);
	_text.appendText(text, 0, _text.height() + 6, _width - 40);
	_height += _text.height() >= 32 ? _text.height() - 32 : 0;
	_y = (SCREEN_HEIGHT - _height) / 2;

	initWidgets();
}

MessageBoxWindow::MessageBoxWindow(GuiView *parent, unsigned help_id,
	const uint8_t *palette, unsigned flags) : GuiWindow(parent, flags) {

	unsigned twidth, y = 0;
	ImageAsset icon;
	const HelpText *entry;

	initAssets();

	do {
		entry = gameLang->help(help_id);
		twidth = _width - 40;

		if (entry->archive) {
			icon = gameAssets->getImage(entry->archive,
				entry->asset_id, palette);
			_text.addSprite(_width - 40 - icon->width(), y,
				(Image*)icon, entry->frame);
			twidth -= icon->width() + 10;
		}

		if (entry->title[0] != 0x14) {
			_text.setFont(FONTSIZE_BIG, FONT_COLOR_HELP);
			_text.appendText(entry->title, 0, y, twidth);
		}

		y = _text.height() + 6;
		y = y < 30 ? 30 : y;
		_text.setFont(FONTSIZE_SMALL, FONT_COLOR_HELP);
		_text.appendText(entry->text, 0, y, _width - 40);
		y = _text.height() + 7;
		help_id = entry->nextParagraph;
	} while (help_id);

	_height += _text.height() >= 20 ? _text.height() - 20 : 0;
	_y = (SCREEN_HEIGHT - _height) / 2;

	initWidgets();
}

MessageBoxWindow::MessageBoxWindow(GuiView *parent, Technology tech,
	unsigned cost, unsigned flags) : GuiWindow(parent, flags) {
	const HelpText *entry;
	const char *str;
	StringBuffer buf;

	initAssets();
	entry = gameLang->help(tech);
	_text.setFont(FONTSIZE_TITLE, TITLE_COLOR_HELP, 2, OUTLINE_NONE, 2);
	_text.appendText(entry->title, 0, 0, _width - 40, ALIGN_CENTER);
	_text.setFont(FONTSIZE_MEDIUM, FONT_COLOR_HELP, 2, OUTLINE_NONE, 2);
	_text.appendText(entry->text, 0, _text.height() + 6, _width - 40,
		ALIGN_JUSTIFY);
	str = gameLang->misctext(TXT_MISC_BILLTEXT, BILL_RESEARCH_COST);
	buf.printf("%s%u RP", str, cost);
	_text.appendText(buf.c_str(), 0, _text.height() + 3, _width - 40,
		ALIGN_CENTER);
	_height += _text.height() >= 32 ? _text.height() - 32 : 0;
	_y = (SCREEN_HEIGHT - _height) / 2;

	initWidgets();
}

MessageBoxWindow::~MessageBoxWindow(void) {

}

void MessageBoxWindow::initAssets(void) {
	_header = gameAssets->getImage(TEXTBOX_ARCHIVE, ASSET_TEXTBOX_HEADER);
	_body = gameAssets->getImage(TEXTBOX_ARCHIVE, ASSET_TEXTBOX_BODY,
		_header->palette());
	_footer = gameAssets->getImage(TEXTBOX_ARCHIVE, ASSET_TEXTBOX_FOOTER,
		_header->palette());
	_width = _header->width();
	_height = _header->height() + _footer->height();
	_x = (SCREEN_WIDTH - _width) / 2;
}

void MessageBoxWindow::initWidgets(void) {
	Widget *w = NULL;

	w = createWidget(158, _height - 27, 64, 19);
	w->setClickSprite(MBUTTON_LEFT, TEXTBOX_ARCHIVE, ASSET_TEXTBOX_BUTTON,
		_header->palette(), 1);
	w->setMouseUpCallback(MBUTTON_LEFT,
		GuiMethod<GuiWindow>(*this, &MessageBoxWindow::close));
}

void MessageBoxWindow::redraw(unsigned curtick) {
	unsigned y, by = _header->height(), fh = _footer->height();

	gameScreen->fillRect(_x + 9, _y + 9, _width - 18, _height - 40,
		16, 16, 24);

	for (y = 10; y < _height - 31; y += 3) {
		gameScreen->fillRect(_x + 9, _y + y, _width - 18, 1,
			36, 36, 40);
	}

	_header->draw(_x, _y);
	gameScreen->drawTextureTile(_body->textureID(0), _x, _y + by, 0, 0,
		_width, _height - by - fh);
	_footer->draw(_x, _y + _height - fh);
	_text.redraw(_x + 20, _y + 11, curtick);
	redrawWidgets(_x, _y, curtick);
}

ConfirmationWindow::ConfirmationWindow(GuiView *parent, const char *text,
	unsigned flags) : GuiWindow(parent, flags) {

	_bg = gameAssets->getImage(CONFIRMATION_ARCHIVE,
		ASSET_CONFIRM_BACKGROUND);

	_width = _bg->width();
	_height = _bg->height();
	_x = (SCREEN_WIDTH - _width) / 2;
	_y = (SCREEN_HEIGHT - _height) / 2;

	_text.setFont(FONTSIZE_BIG, FONT_COLOR_FLEETLIST_STAR_ORANGE, 2,
		OUTLINE_NONE, 2);
	_text.appendText(text, 0, 0, CONFIRM_TEXT_WIDTH, ALIGN_CENTER);
	initWidgets();
}

void ConfirmationWindow::initWidgets(void) {
	Widget *w;
	const uint8_t *pal = _bg->palette();

	w = createWidget(74, 185, 51, 21);
	w->setIdleSprite(CONFIRMATION_ARCHIVE, ASSET_CONFIRM_YES_BUTTON, pal,
		0);
	w->setClickSprite(MBUTTON_LEFT, CONFIRMATION_ARCHIVE,
		ASSET_CONFIRM_YES_BUTTON, pal, 1);
	w->setMouseUpCallback(MBUTTON_LEFT,
		GuiMethod(*this, &ConfirmationWindow::clickYes));

	w = createWidget(184, 185, 51, 22);
	w->setIdleSprite(CONFIRMATION_ARCHIVE, ASSET_CONFIRM_NO_BUTTON, pal, 0);
	w->setClickSprite(MBUTTON_LEFT, CONFIRMATION_ARCHIVE,
		ASSET_CONFIRM_NO_BUTTON, pal, 1);
	w->setMouseUpCallback(MBUTTON_LEFT,
		GuiMethod(*this, &ConfirmationWindow::clickNo));
}

void ConfirmationWindow::setYesCallback(const GuiCallback &callback) {
	_onYes = callback;
}

void ConfirmationWindow::setNoCallback(const GuiCallback &callback) {
	_onNo = callback;
}

void ConfirmationWindow::clickYes(int x, int y, int arg) {
	_onYes(x, y);
	close(x, y, arg);
}

void ConfirmationWindow::clickNo(int x, int y, int arg) {
	_onNo(x, y);
	close(x, y, arg);
}

void ConfirmationWindow::redraw(unsigned curtick) {
	int offset;

	offset = (CONFIRM_TEXT_HEIGHT - (int)_text.height()) / 2;
	_bg->draw(_x, _y);
	_text.redraw(_x + CONFIRM_TEXT_X, _y + CONFIRM_TEXT_Y + offset,
		curtick);
	redrawWidgets(_x, _y, curtick);
}

ErrorWindow::ErrorWindow(GuiView *parent, const char *text) :
	GuiWindow(parent, WINDOW_MODAL), _animStart(0) {

	_bg = gameAssets->getImage(ERROR_ARCHIVE, ASSET_ERROR_BACKGROUND);
	_width = _bg->width();
	_height = _bg->height();
	_x = (SCREEN_WIDTH - _width) / 2;
	_y = (SCREEN_HEIGHT - _height) / 2;

	_text.setFont(FONTSIZE_BIG, FONT_COLOR_ERROR);
	_text.appendText(text, 0, 0, 227, ALIGN_CENTER);
}

ErrorWindow::~ErrorWindow(void) {

}

void ErrorWindow::redraw(unsigned curtick) {
	unsigned frame;
	int y;

	if (!_animStart) {
		_animStart = curtick;
	}

	// FIXME: calculate frame time from image header
	// Original game plays 14 frames in ~1.3 seconds
	frame = bounceFrame(curtick - _animStart, 100, _bg->frameCount());
	_bg->draw(_x, _y, frame);
	y = _y + (_height - _text.height()) / 2;
	_text.redraw(_x + 53, y, curtick);
}

void ErrorWindow::handleMouseUp(int x, int y, unsigned button) {
	close();
}

void drawFrame(int x, int y, unsigned width, unsigned height,
	const uint8_t *clr) {
	gameScreen->fillRect(x, y, width, 1, clr[0], clr[1], clr[2]);
	gameScreen->fillRect(x + width - 1, y + 1, 1, height - 2, clr[0],
		clr[1], clr[2]);
	clr += 3;
	gameScreen->fillRect(x + 2, y + 1, width - 3, 1, clr[0], clr[1],
		clr[2]);
	gameScreen->fillRect(x + width - 2, y + 2, 1, height - 4, clr[0],
		clr[1], clr[2]);
	clr += 3;
	gameScreen->fillRect(x, y + 1, 1, height - 1, clr[0], clr[1], clr[2]);
	gameScreen->fillRect(x + 1, y + height - 1, width - 1, 1, clr[0],
		clr[1], clr[2]);
	clr += 3;
	gameScreen->fillRect(x + 1, y + 1, 1, height - 2, clr[0], clr[1],
		clr[2]);
	gameScreen->fillRect(x + 2, y + height - 2, width - 3, 1, clr[0],
		clr[1], clr[2]);
}

void drawETA(int x, int y, unsigned color, const char *str) {
	unsigned w;
	Font *fnt;
	const uint8_t *pal;

	fnt = gameFonts->getFont(FONTSIZE_SMALL);
	w = fnt->textWidth(str, 2);
	x -= w / 2;
	y -= fnt->height() / 2;
	pal = Font::fontPalette(color);

	// double outline
	fnt->renderText(x + 1, y + 1, color, str, OUTLINE_FULL, 2);
	fnt->renderText(x, y, color, str, OUTLINE_FULL, 2);
	gameScreen->fillRect(x - 1, y - 4, w + 3, 1, pal[9], pal[10], pal[11]);
	gameScreen->fillRect(x - 1, y - 3, w + 3, 1, pal[5], pal[6], pal[7]);
	y += fnt->height();
	gameScreen->fillRect(x - 1, y, w + 3, 1, pal[5], pal[6], pal[7]);
	gameScreen->fillRect(x - 1, y + 1, w + 3, 1, pal[9], pal[10], pal[11]);

}

unsigned spriteSpacing(unsigned maxWidth, unsigned spriteWidth,
	unsigned count, unsigned maxSpace) {

	unsigned ret = maxWidth;

	if (count < 2 || ret < spriteWidth + count) {
		return 1;
	}

	ret -= spriteWidth;
	ret /= count - 1;
	return MIN(ret, spriteWidth + maxSpace);
}
