#ifndef fileTasks
#define fileTasks

#include "../dialogs/dialogHeader.hpp"
#include "editorTasks.hpp"
#include "../integration/interpreter/interpreter.hpp"

void fileOpenTask(const std::string& fileName);
void fileSaveAsTask(const std::string &fileName);
void fileSaveTask(const std::string &fileName);
extern void fileRunTask(uint64_t instructionCount = 0);
#endif // !fileTasks
