#include <Hydrogen/Hydrogen.hpp>

int main() {
    Hydrogen::Logger logger("Test", Hydrogen::Logger::LogLevel::Debug);
    logger.Warn("test");
}