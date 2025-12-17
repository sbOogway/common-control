#include "include/common-control.h"

int main()
{
  // Initialize the logging system
  log_init();

  // Configure logging
  log_set_level(LOG_DEBUG);  // Show all messages from DEBUG level and above
  log_set_output(LOG_OUTPUT_CONSOLE
                 | LOG_OUTPUT_FILE);  // Log to both console and file
  log_set_file("application.log");  // Set log file name

  // Use the logging macros throughout your application
  LOG_DEBUG("Application starting up");
  LOG_INFO("User logged in successfully");
  LOG_WARN("Configuration file not found, using defaults");
  LOG_ERROR("Database connection failed");

  // You can use format strings like printf
  int user_id = 12345;
  const char* username = "john_doe";
  LOG_INFO("User %s (ID: %d) performed an action", username, user_id);

  // Cleanup when done
  log_cleanup();

  return 0;
}
