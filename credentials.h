/*
 * Setup for Weather Display
 */

// WiFi credentials
const char* ssid         = "<Your SSID>";                           // WiFi SSID
const char* password     = "<Your SSID password>";                  // WiFi password

// Dark Sky Developer Account
String apikey            = "<Your Dark Sky Developer Key>";         // Get the developer key for 1000 request/day
const char server[]      = "api.darksky.net";                       // URL (don't change)

// Your Location
const float longitude    = <Latitude of your location>;             // Latitude
const float latitude     = <Longitude of your location>;            // Longitude
const float altitude     = <Elevation of your location>;            // altitude of your location in meters

// API Parameters
const char* exclude      = "minutely,hourly,flags";                 // Reduce the size of the returned JSON (don't change)
const char* excludeHour  = "currently,minutely,daily,alerts,flags"; // Reduce the size of the returned JSON (don't change)
const char* units        = "us";                                    // units "us", "auto", "ca", "uk2", "us", "si"
const char* language     = "en";                                    // language

// GMT offset in seconds
int gmtOffset_sec        = <Timezone offset GMT>;                   // Timezone offset of your location in seconds

// Sleep Time in Minutes
const long SleepDuration = 30;                                      // Display update interval
const int PowerSaveStart = 22;                                      // Start power save where display is not updated
const int PowerSaveEnd   = 5;                                       // End power save where display is not updated
