#pragma once

#define BIT(x) (1 << x)
#define FILENAME_FROM_PATH(path) path.substr(path.find_last_of("/\\") + 1)
#define DIRECTORY_FROM_PATH(path) path.substr(0, path.find_last_of("\\/"))

#define MAX_FRAMES_IN_FLIGHT 3
