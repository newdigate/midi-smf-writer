//
// Created by Moolet on 25/01/2025.
//

#ifndef MIDI_SMF_WRITER_DELTATIMESEQUENCER_H
#define MIDI_SMF_WRITER_DELTATIMESEQUENCER_H
class DeltaTimeSequencer {
public:
    DeltaTimeSequencer(unsigned microsPerTick, bool startTimingOnFirstEvent) :
            _microsPerTick(microsPerTick),
            _startTimingOnFirstEvent(startTimingOnFirstEvent) {
    }
    DeltaTimeSequencer(double atempo, unsigned short ticksPerBeat, bool startTimingOnFirstEvent) :
            _microsPerTick(calculateMicrosPerTick(atempo, ticksPerBeat)),
            _startTimingOnFirstEvent(startTimingOnFirstEvent) {
    }

    void stop() {
        _startMicroseconds = 0;
        _lastTick = 0;
    }

    void start(unsigned long currentMicros) {
        _startMicroseconds = currentMicros;
        _lastTick = 0;
    }

    void pause(unsigned long currentMicros) {
        if (_startMicroseconds == 0)
            return;
        if (_paused) return;
        _pausedMicroseconds = currentMicros;
        auto const pausedTicks = (currentMicros - _startMicroseconds)  / _microsPerTick;
        _remainderTicks = pausedTicks - _lastTick;
        _paused = true;
    }

    void unpause(unsigned long currentMicros) {
        if (_startMicroseconds == 0)
            return;
        if (!_paused) return;
        auto ticks = (currentMicros - _startMicroseconds) / _microsPerTick;
        _lastTick = ticks - _remainderTicks;
        _paused = false;
    }

    unsigned get_delta(unsigned long currentMicros) {
        if (_startMicroseconds == 0) {
            start(currentMicros);
            if (_startTimingOnFirstEvent)
                return 0;
        }

        const auto deltaMicros = currentMicros - _startMicroseconds;
        unsigned ticks = deltaMicros / _microsPerTick;

        if (_paused) {
            if (_remainderTicks > 0) {
                auto result = _remainderTicks;
                _remainderTicks = 0;
                return result;
            }
            return 0;
        }

        auto delta = ticks - _lastTick;
        _lastTick = ticks;
        return delta;
    }

    static unsigned int calculateMicrosPerTick(double bpm, unsigned short ticksPerBeat) {
        return (60000000.0 / bpm) / ticksPerBeat;
    }

    unsigned long getMicroseconds(unsigned long currentMicros) const {
        if (_paused)
            return _pausedMicroseconds - _startMicroseconds;
        return currentMicros - _startMicroseconds;
    }

    unsigned long getMicrosPerTick() const {
        return _microsPerTick;
    }

    unsigned long getInactivityMicros(const unsigned long currentMicros ) {
        if (_startMicroseconds == 0) return 0;
        if (_lastTick==0) return currentMicros -_startMicroseconds;
        return currentMicros - (_lastTick * _microsPerTick) -_startMicroseconds;
    }

private:
    unsigned long _startMicroseconds = 0, _lastTick = 0, _microsPerTick = 0, _remainderTicks = 0, _pausedMicroseconds = 0;
    bool _startTimingOnFirstEvent, _paused = false;
};

#endif //MIDI_SMF_WRITER_DELTATIMESEQUENCER_H
