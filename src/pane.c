/* MUIL - Muon UI Library
 * This file is part of MUIL.
 * Copyright 2012-2017 Axel Isaksson and Steven Arnow.
 * 
 * The original code in Muon was licensed as GPL, the MUIL parts have been relicensed as MIT license.
 * See the COPYING file for details
 */


#include "muil.h"

MuilPane *muil_pane_create(int x, int y, int w, int h, MuilWidget *root_widget) {
	MuilPane *pane;
	if((pane = malloc(sizeof(MuilPane))) == NULL)
		return NULL;
	pane->x = x;
	pane->y = y;
	pane->w = w;
	pane->h = h;
	pane->background = draw_rect_set_new(1);
	pane->background_title = draw_rect_set_new(1);
	
	
	pane->border_dark = draw_line_set_new(2, 1);
	pane->border_shadow = draw_line_set_new(7, 1);
	pane->border = draw_line_set_new(4, 1);
	pane->border_highlight = draw_line_set_new(7, 1);
	
	pane->root_widget = NULL;
	pane->needs_redraw = true;
	muil_pane_resize(pane, x, y, w , h);
	muil_pane_set_root_widget(pane, root_widget);
	
	pane->title = NULL;
	
	return pane;
}

MuilPane *muil_pane_create_title(int x, int y, int w, int h, MuilWidget *root_widget, char *title) {
	MuilPane *pane;
	
	if(!(pane = muil_pane_create(x, y, w, h, root_widget)))
		return NULL;
	
	pane->title = title;
	
	return pane;
}

void *muil_pane_destroy(MuilPane *pane) {
	draw_line_set_free(pane->border);
	draw_rect_set_free(pane->background);
	free(pane);
	return NULL;
}

void muil_pane_resize(MuilPane *pane, int x, int y, int w, int h) {
	int title_h;
	
	pane->x = x;
	pane->y = y;
	pane->w = w;
	pane->h = h;
	pane->needs_redraw = true;
	
	title_h = 16;
	
	draw_rect_set_move(pane->background_title, 0, x, y, x + w, y + h);
	draw_rect_set_move(pane->background, 0, x + muil_padding*2, y + title_h + muil_padding*2, x + w - muil_padding*2, y + h - muil_padding*2);
	
	/* Dark shadow */
	draw_line_set_move(pane->border_dark, 0, x, y + h, x + w, y + h);
	draw_line_set_move(pane->border_dark, 1, x + w, y, x + w, y + h);
	
	/* Shadow */
	draw_line_set_move(pane->border_shadow, 0, x + 1, y + h - 1, x + w - 1, y + h - 1);
	draw_line_set_move(pane->border_shadow, 1, x + w - 1, y + 1, x + w - 1, y + h - 1);
	draw_line_set_move(pane->border_shadow, 2, x + muil_padding*2 - 2, y + title_h + muil_padding*2 - 2, x + muil_padding*2 - 2, y + h - muil_padding*2);
	draw_line_set_move(pane->border_shadow, 3, x + muil_padding*2 - 2, y + title_h + muil_padding*2 - 2, x + w - muil_padding*2, y + title_h + muil_padding*2 - 2);
	
	draw_line_set_move(pane->border_shadow, 4, x + muil_padding*2, y + muil_padding + 1, x + w - muil_padding*2, y + muil_padding + 1);
	draw_line_set_move(pane->border_shadow, 5, x + muil_padding*2, y + muil_padding + (muil_padding + 2) + 1, x + w - muil_padding*2, y + muil_padding + (muil_padding + 2) + 1);
	draw_line_set_move(pane->border_shadow, 6, x + muil_padding*2, y + muil_padding + (muil_padding + 2)*2 + 1, x + w - muil_padding*2, y + muil_padding + (muil_padding + 2)*2 + 1);
	
	/* Border */
	draw_line_set_move(pane->border, 0, x, y, x, y + h - 1);
	draw_line_set_move(pane->border, 1, x, y, x + w - 1, y);
	
	/* Highlight */
	draw_line_set_move(pane->border_highlight, 0, x + 1, y + 1, x + 1, y + h - 2);
	draw_line_set_move(pane->border_highlight, 1, x + 1, y + 1, x + w - 2, y + 1);
	draw_line_set_move(pane->border_highlight, 2, x + (muil_padding*2 - 2), y + h - (muil_padding*2 - 1), x + w - (muil_padding*2 - 1), y + h - (muil_padding*2 - 1));
	draw_line_set_move(pane->border_highlight, 3, x + w - (muil_padding*2 - 1), y + title_h + muil_padding*2 - 2, x + w - (muil_padding*2 - 1), y + h - (muil_padding*2 - 1));
	
	draw_line_set_move(pane->border_highlight, 4, x + muil_padding*2, y + muil_padding + 2, x + w - muil_padding*2, y + muil_padding + 2);
	draw_line_set_move(pane->border_highlight, 5, x + muil_padding*2, y + muil_padding + (muil_padding + 2) + 2, x + w - muil_padding*2, y + muil_padding + (muil_padding + 2) + 2);
	draw_line_set_move(pane->border_highlight, 6, x + muil_padding*2, y + muil_padding + (muil_padding + 2)*2 + 2, x + w - muil_padding*2, y + muil_padding + (muil_padding + 2)*2 + 2);
	
	if(pane->root_widget != NULL)
		pane->root_widget->resize(pane->root_widget, x +muil_padding*3, y + muil_padding*3 + title_h, w - muil_padding*6, h - muil_padding*6 - title_h);
}

void muil_pane_set_root_widget(MuilPane *pane, MuilWidget *root_widget) {
	int title_h;
	title_h = 16;
	
	pane->root_widget = root_widget;
	if(pane->root_widget != NULL)
		pane->root_widget->resize(pane->root_widget, pane->x +muil_padding*3, pane->y +muil_padding*3 + title_h, pane->w - muil_padding*6, pane->h - muil_padding*6 - title_h);
}

void muil_pane_render(MuilPane *pane) {
	if(pane->needs_redraw) {
		draw_set_color(muil_color.window_border);
		draw_rect_set_draw(pane->background_title, 1);
		draw_set_color(muil_color.window_background);
		draw_rect_set_draw(pane->background, 1);
		
		draw_set_color(muil_color.widget_border_shadow_strong);
		draw_line_set_draw(pane->border_dark, 2);
		draw_set_color(muil_color.widget_border_shadow);
		draw_line_set_draw(pane->border_shadow, 7);
		draw_set_color(muil_color.widget_border);
		draw_line_set_draw(pane->border, 2);
		draw_set_color(muil_color.widget_border_highlight);
		draw_line_set_draw(pane->border_highlight, 7);
		
		pane->needs_redraw = false;
	}

	if(pane->root_widget != NULL)
		pane->root_widget->render(pane->root_widget);
}

void muil_panelist_render(struct MuilPaneList *panelist) {
	for(; panelist; panelist = panelist->next)
		muil_pane_render(panelist->pane);
}
