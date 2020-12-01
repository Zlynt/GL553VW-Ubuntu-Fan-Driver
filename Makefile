all: clean compile

clean:
	@echo Cleaning...
	@rm -rf build
	@mkdir build
	@echo Done!
compile:
	@echo Compiling...
	@gcc fan.c -o ./build/fan
	@echo Done!
run:
	sudo modprobe acpi_call
	sudo ./build/fan