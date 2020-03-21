launchpadTest : main.cpp RtMidi.cpp
	g++ -Wall -D__LINUX_ALSA__ -o out main.cpp RtMidi.cpp -lasound -lpthread
