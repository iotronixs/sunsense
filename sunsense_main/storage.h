/**
 * @file storage.h
 * @brief This header file defines the Session struct for storing UV exposure data and declares functions for initializing, saving, and retrieving sessions in the Sunsense application.
 * @author @jpg.py
 * @date 2026-04-27
 */

/**
 * @struct Session
 * @brief Represents a user session with UV exposure metrics.
 * @details Stores average UV level, maximum UV level, session duration, and SPF value.
 */

/**
 * @brief Initializes the storage system for sessions.
 */

#ifndef STORAGE_H
#define STORAGE_H

struct Session {
  float avgUV;
  float maxUV;
  float duration;
  int spf;
};

void initStorage();
void saveSession(Session s);
int getSessionCount();
Session getSession(int index);

#endif