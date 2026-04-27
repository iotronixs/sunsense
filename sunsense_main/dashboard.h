/**
 * @file dashboard.h
 * @brief Header file for the dashboard module, providing functions to start and handle the dashboard display for UV exposure monitoring.
 * @author @jpg.py
 * @date 2026-27-04
 */

/**
 * @brief Starts the dashboard interface.
 * This function initializes and launches the dashboard, setting up necessary components for displaying UV-related information.
 */

/**
 * @brief Handles the dashboard updates based on UV index, time left, and SPF value.
 * @param uv The current UV index value.
 * @param timeLeft The remaining time before UV exposure limit is reached.
 * @param spf The Sun Protection Factor value.
 * This function updates the dashboard display with the provided parameters to inform the user about sun exposure safety.
 */

#ifndef DASHBOARD_H
#define DASHBOARD_H

void startDashboard();
void handleDashboard(float uv, float timeLeft, int spf);

#endif

