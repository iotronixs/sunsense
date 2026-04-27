
/**
 * @file dashboard.cpp
 * @brief This file implements a web-based dashboard for a UV sensing device using an ESP32 microcontroller. It sets up a WiFi Access Point (AP) and serves a web server that displays live UV index, time left for sun exposure, SPF value, risk messages, recommendations, advice, session summaries, and a graphical representation of exposure history. The dashboard includes dynamic HTML pages with CSS styling for a user-friendly interface, handling real-time data updates and session storage retrieval.
 * 
 * Key functionalities include:
 * - Risk assessment based on UV levels and time left.
 * - Generation of HTML for the main dashboard page with live status, recommendations, advice, and session history in a table format.
 * - A separate graph page displaying session durations as bars.
 * - AP setup for wireless connectivity, allowing devices to connect and access the dashboard.
 * - Handling of client requests in a loop, updating live variables.
 * 
 * The code utilizes libraries such as WiFi.h and WebServer.h for network operations, and integrates with custom modules like storage.h for session data management.
 * 
 * @author @jpg.py
 * @date 2026-27-04
 * @board esp32-c3 supermini
 */

 
#include "dashboard.h"
#include "storage.h"
#include <WiFi.h>
#include <WebServer.h>

WebServer server(80);

float liveUV = 0;
float liveTime = 0;
int liveSPF = 0;

// ===== RISK MESSAGE =====
String getRiskMessage(float uv, float timeLeft) {

  if (timeLeft <= 10)
    return "Reapply sunscreen immediately";

  if (uv > 7 && timeLeft < 30)
    return "Danger: Prolonged high UV exposure";

  if (uv > 5)
    return "High UV: Limit sun exposure";

  if (uv > 3)
    return "Moderate UV: Use protection";

  return "Low risk";
}

// ===== GRAPH PAGE =====
void handleGraph() {

  String html = "<html><head>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1'>";

  html += "<style>";
  html += "body{font-family:Arial;background:#0f172a;color:white;text-align:center;}";
  html += ".bar{background:#22c55e;margin:5px auto;height:20px;border-radius:5px;}";
  html += "</style>";

  html += "</head><body>";

  html += "<h2>Exposure Graph</h2>";

  int count = getSessionCount();

  for (int i = 0; i < count; i++) {
    Session s = getSession(i);

    int width = s.duration * 2;  // scale

    html += "<div>" + String(s.duration) + " min</div>";
    html += "<div class='bar' style='width:" + String(width) + "px'></div>";
  }

  html += "<br><a href='/'>Back</a>";

  html += "</body></html>";

  server.send(200, "text/html", html);
}

// ===== MAIN DASHBOARD =====
void handleRoot() {

  String html = "<html><head>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
  html += "<meta http-equiv='refresh' content='2'>";

  // ===== CSS =====
  html += "<style>";
  html += "body{font-family:Arial;background:#0f172a;color:white;margin:0;}";
  html += ".container{max-width:500px;margin:auto;padding:10px;}";
  html += ".card{background:#1e293b;padding:15px;margin-bottom:12px;border-radius:12px;}";
  html += ".title{font-size:14px;color:#94a3b8;margin-bottom:6px;}";
  html += ".big{font-size:32px;font-weight:bold;}";
  html += ".danger{color:#ef4444;font-weight:bold;}";
  html += ".warn{color:#f59e0b;font-weight:bold;}";
  html += ".safe{color:#22c55e;font-weight:bold;}";

  html += "table{width:100%;border-collapse:collapse;}";
  html += "th,td{text-align:center;padding:8px;}";
  html += "th{background:#334155;}";
  html += "tr:nth-child(even){background:#1e293b;}";

  html += "button{padding:10px;border:none;border-radius:8px;background:#22c55e;color:black;font-weight:bold;}";
  html += "</style>";

  html += "</head><body><div class='container'>";

  // ===== LIVE =====
  html += "<div class='card'>";
  html += "<div class='title'>Live Status</div>";
  html += "<div class='big'>UV " + String(liveUV,1) + "</div>";
  html += "Time Left: " + String(liveTime,0) + " min<br>";
  html += "SPF: " + String(liveSPF);
  html += "</div>";

  // ===== MESSAGE =====
  String msg = getRiskMessage(liveUV, liveTime);

  html += "<div class='card'>";
  html += "<div class='title'>Recommendation</div>";

  if (liveTime <= 10) html += "<div class='danger'>";
  else if (liveUV > 5) html += "<div class='warn'>";
  else html += "<div class='safe'>";

  html += msg;
  html += "</div></div>";

  // ===== ADVICE =====
  html += "<div class='card'>";
  html += "<div class='title'>Advice</div>";

  if (liveUV > 6)
    html += "Avoid direct sunlight<br>";

  if (liveTime < 20)
    html += "Reapply sunscreen<br>";

  html += "Stay hydrated";
  html += "</div>";

  // ===== SUMMARY =====
  int count = getSessionCount();

  float totalUV = 0;
  float totalTime = 0;

  for (int i = 0; i < count; i++) {
    Session s = getSession(i);
    totalUV += s.avgUV;
    totalTime += s.duration;
  }

  html += "<div class='card'>";
  html += "<div class='title'>Summary</div>";

  if (count == 0) {
    html += "No data yet";
  } else {

    float avgUVAll = totalUV / count;

    html += "Sessions: " + String(count) + "<br>";
    html += "Avg UV: " + String(avgUVAll,1) + "<br>";
    html += "Total Exposure: " + String(totalTime) + " min<br>";

    if (avgUVAll > 6)
      html += "<div class='danger'>High overall UV exposure</div>";
    else if (avgUVAll > 4)
      html += "<div class='warn'>Moderate exposure</div>";
    else
      html += "<div class='safe'>Low exposure</div>";

    if (avgUVAll > 6 && totalTime > 120)
      html += "<div class='danger'>You are consistently overexposed</div>";

    if (avgUVAll > 5 && liveSPF < 40)
      html += "<div class='warn'>Consider higher SPF</div>";
  }

  html += "</div>";

  // ===== GRAPH BUTTON =====
  html += "<div class='card'>";
  html += "<a href='/graph'><button>View Graph</button></a>";
  html += "</div>";

  // ===== TABLE =====
  html += "<div class='card'>";
  html += "<div class='title'>Session History</div>";

  if (count > 0) {

    html += "<table>";
    html += "<tr><th>Avg UV</th><th>Max</th><th>Time</th><th>SPF</th></tr>";

    for (int i = count - 1; i >= 0; i--) {
      Session s = getSession(i);

      html += "<tr>";
      html += "<td>" + String(s.avgUV,1) + "</td>";
      html += "<td>" + String(s.maxUV,1) + "</td>";
      html += "<td>" + String(s.duration) + "</td>";
      html += "<td>" + String(s.spf) + "</td>";
      html += "</tr>";
    }

    html += "</table>";
  }

  html += "</div>";

  html += "</div></body></html>";

  server.send(200, "text/html", html);
}

// ===== START AP =====
void startDashboard() {

  WiFi.disconnect(true, true);
  delay(2000);

  WiFi.mode(WIFI_AP);
  WiFi.setSleep(false);
  WiFi.setTxPower(WIFI_POWER_8_5dBm);

  WiFi.softAP("UVSense", "12345678", 1, 0, 4);

  server.on("/", handleRoot);
  server.on("/graph", handleGraph);  // 🔥 NEW

  server.begin();

  Serial.println("AP Started");
  Serial.print("IP: ");
  Serial.println(WiFi.softAPIP());
}

// ===== LOOP =====
void handleDashboard(float uv, float timeLeft, int spf) {

  liveUV = uv;
  liveTime = timeLeft;
  liveSPF = spf;

  server.handleClient();
}