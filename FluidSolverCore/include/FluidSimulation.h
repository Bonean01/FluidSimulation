#pragma once

class FluidSimulation {
public:
	const bool running() const { return isRunning; }
	void stop() { isRunning = false; }
	FluidSimulation() : isRunning(true) {}

private:
	bool isRunning;
};
