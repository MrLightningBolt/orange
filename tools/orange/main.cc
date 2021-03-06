/*
** Copyright 2014-2015 Robert Fratto. See the LICENSE.txt file at the top-level 
** directory of this distribution.
**
** Licensed under the MIT license <http://opensource.org/licenses/MIT>. This file 
** may not be copied, modified, or distributed except according to those terms.
*/ 
#include <iostream>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <cereal/types/unordered_map.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/archives/json.hpp>
#include <helper/args.h>
#include <helper/link.h>
#include <orange/file.h>
#include <orange/run.h>
#include <orange/build.h>
#include <orange/test.h>
#include <orange/orange.h>

int main(int argc, char** argv) {
	cOptions options("Orange WIP"); 

	/*
	 * Set up our "run" state to run files.
	 */
	cOptionsState run("run", "Runs a single file or a project.", "run [filename]", "The run command will\
 either run a file or a project directly. Entering run without a filename will run a project, if it exists.\
  If you are not in a project, an error will be displayed. Running a file does not require you to be in an Orange project.");

  cCommandOption debug({"debug", "D"}, "Print debugging info", false);
 	run.add(&debug);
 	options.mainState.add(&debug);

	options.mainState.addState(&run); 

	/**
	 * Set up our "build" state to compile files.
	 */
	cOptionsState build("build", "Compiles a single file or project", "build [filename]", "The build command will\
 either compile a file or a project directly. Entering compile without a filename will compile a project, if it exists.\
  If you are not in a project, an error will be displayed. Building a file does not require you to be in an Orange project.");
	build.add(&debug);

	options.mainState.addState(&build);

	/*
	 * Set up our "test" state to test files.
	 */
	cOptionsState test("test", "Tests files and projects in the test/ directory.", "test [folder|filename|project]", "The test command\
 is used to test every file and project inside of the test/ folder. It will run recursively, through each subdirectory. If a subdirectory\
 contains a orange.settings.json file, it is treated as a sub project. Otherwise, every file inside of the directory will be ran as its own\
  individual program.");
  options.mainState.addState(&test);

	// Parse our options
	options.parse(argc, argv);

	if (run.isActive()) {
		doRunCommand(run, debug.isSet());
	} else if (test.isActive()) {
		doTestCommand(test);
	} else if (build.isActive()) {
		doBuildCommand(build, debug.isSet());
	} else {
		doRunCommand(options.mainState, debug.isSet());
	}

	return 0;
}

