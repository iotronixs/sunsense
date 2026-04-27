
/**
 * @file storage.cpp
 * @brief Implements session persistence using ESP32-c3 supermini Preferences storage.
 *
 * This module initializes a non-volatile key-value storage namespace,
 * stores Session objects in sequential slots, tracks the number of stored
 * sessions, and retrieves sessions by index.
 *
 * Functionality:
 * - initStorage: opens the Preferences namespace for read/write access.
 * - saveSession: writes a Session to persistent storage, overwriting the
 *   oldest entry when the maximum session count is reached.
 * - getSessionCount: returns the current number of stored sessions.
 * - getSession: reads a Session from storage by its index.
 *
 * Uses a fixed maximum session count and stores sessions under keys
 * formatted as "s0", "s1", ..., with a persistent count maintained
 * under the key "count".
 *
 * @author @jpg.py
 * @date 2026-27-04
 */
#include "storage.h"
#include <Preferences.h>

Preferences prefs;

#define MAX_SESSIONS 20

void initStorage() {
  prefs.begin("uvstore", false);
}

void saveSession(Session s) {

  int count = prefs.getInt("count", 0);

  if (count >= MAX_SESSIONS) count = 0; // overwrite oldest

  String key = "s" + String(count);

  prefs.putBytes(key.c_str(), &s, sizeof(Session));

  count++;
  prefs.putInt("count", count);
}

int getSessionCount() {
  return prefs.getInt("count", 0);
}

Session getSession(int index) {
  Session s;
  String key = "s" + String(index);

  prefs.getBytes(key.c_str(), &s, sizeof(Session));

  return s;
}