#!/bin/bash

# Default ESP-IDF path
IDF_PATH="$HOME/esp-idf"

# Colors
GREEN='\033[0;32m'
RED='\033[0;31m'
CYAN='\033[1;36m'
NC='\033[0m' # No color

# Help message
show_help() {
    echo -e "${CYAN}ESP-IDF Build Script${NC}"
    echo -e "Usage: ./esp_build.sh [option]\n"
    echo "Options:"
    echo "  build        Build the project (default if no argument is given)"
    echo "  flash        Build and flash the project to /dev/ttyUSB0"
    echo "  monitor      Build, flash, and open the serial monitor"
    echo "  clean        Run idf.py fullclean"
    echo "  help         Show this help message"
}

# Check if ESP-IDF is installed
if [ ! -d "$IDF_PATH" ]; then
    echo -e "${RED}ERROR:${NC} ESP-IDF not found at $IDF_PATH"
    exit 1
fi

# Activate ESP-IDF environment
echo -e "${GREEN}Activating ESP-IDF environment...${NC}"
source "$IDF_PATH/export.sh" || {
    echo -e "${RED}ERROR:${NC} Failed to source export.sh"
    exit 1
}

# Default action: build
ACTION="${1:-build}"

# Execute selected action
case "$ACTION" in
    build)
        echo -e "${GREEN}Building project...${NC}"
        idf.py build
        ;;
    flash)
        echo -e "${GREEN}Building and flashing to /dev/ttyUSB0...${NC}"
        idf.py build && idf.py -p /dev/ttyUSB0 flash
        ;;
    monitor)
        echo -e "${GREEN}Building, flashing, and opening monitor on /dev/ttyUSB0...${NC}"
        idf.py build && idf.py -p /dev/ttyUSB0 flash monitor
        ;;
    clean)
        echo -e "${GREEN}Running full clean...${NC}"
        idf.py fullclean
        rm -rf build/
        ;;
    help|-h|--help)
        show_help
        ;;
    *)
        echo -e "${RED}ERROR:${NC} Unknown option: $ACTION"
        show_help
        exit 1
        ;;
esac
