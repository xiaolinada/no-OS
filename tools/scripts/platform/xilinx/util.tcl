proc _get_processor {} {
	set processor [hsi::get_cells * -filter {IP_TYPE==PROCESSOR}]
	if {[llength $processor] != 0} {
		return $processor
	}
	return 0
}

proc _replace_heap {} {
	set file_name "$::env(WORKSPACE)/app/src/lscript.ld"
	set temp_name "$file_name.tmp"
	set file [open [lindex $file_name] r]
	set temp [open [lindex $temp_name] w+]
	while {[gets $file line] >= 0} {
		if {[string first _HEAP_SIZE $line ] != -1} {
			puts $temp "_HEAP_SIZE = 0x100000;"
		}
		puts $temp $line
	}
	close $file
	close $temp

	file rename -force $temp_name $file_name
}

proc _project_config {cmd {arg}} {
	foreach path $::env(EXTRA_INC_PATHS) {
		set new_path [string map [list $::env(PROJECT_BUILD)	\
				  \${ProjDirPath}] $path] 
		$cmd $arg -name app include-path $new_path
	}
	foreach symbol $::env(FLAGS_WITHOUT_D) {
		$cmd $arg -name app define-compiler-symbols $symbol
	}
	foreach lib $::env(EXTRA_LIBS_NAMES) {
		$cmd $arg -name app libraries $lib
	}
	foreach lib_path $::env(EXTRA_LIBS_PATHS) {
		$cmd $arg -name app library-search-path $lib_path
	}
}

proc _vitis_project {} {
	hsi::open_hw_design $::hw -name hw
	set cpu [_get_processor]
	
	# Create bsp
	hsi::generate_bsp						\
		-dir bsp						\
		-proc $cpu						\
		-os standalone						\
		-compile
	hsi::close_hw_design -name hw
	
	# Create app
	app create							\
		-name app						\
		-hw $::hw						\
		-proc $cpu						\
		-os standalone						\
		-template  {Empty Application}

	# Increase heap size
	_replace_heap

	# Configure the project
	_project_config "app" "config"

	# Build the project
	app build -name app
}

proc _xsdk_project {} {
	set hw_design [hsi::open_hw_design $::hw]
	set cpu [_get_processor]
	# # Create hwspec
	sdk createhw							\
		-name hw						\
		-hwspec $::hw
	# Create bsp
	sdk createbsp							\
		-name bsp						\
		-hwproject hw						\
		-proc $cpu						\
		-os standalone  
	# Create app
	sdk createapp							\
		-name app						\
		-hwproject hw						\
		-proc $cpu						\
		-os standalone						\
		-lang C							\
		-app {Empty Application}				\
		-bsp bsp

	# Increase heap size
	_replace_heap

	# Configure the project
	_project_config "sdk" "configapp"

	# Build the project via SDK
	clean -type all
	build -type all

	projects -build -name app -type app
}

proc get_arch {} {
	hsi::open_hw_design $::hw -name hw
	set cpu [_get_processor]
	set file [open [lindex "$::env(WORKSPACE)/tmp/arch.txt"] w+]
	puts $file $cpu
	close $file
	hsi::close_hw_design -name hw
}

proc create_project {} {
	cd $::env(WORKSPACE)
	setws ./
	if {[string first .hdf $::hw ] != -1} {
		_xsdk_project
	} else {
		_vitis_project
	}
}

set function	[lindex $argv 0]
set hw		$::env(WORKSPACE)/tmp/$::env(HARDWARE)

$function
