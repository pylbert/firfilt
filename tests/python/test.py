#! /usr/bin/env python

def main():
    import math
    import fir
    from operator import add

    Fs = 1000.
    T = 1/(Fs) * 2 * math.pi
    DCoffset = 2.
    times = [x*T for x in range(0, 4000)]
    nTaps = 51

    filt = fir.Filter_LP(nTaps, 1000, 5)

    sin_2Hz = [DCoffset + math.sin(t) for t in times]
    sin_20Hz_noise = [.2*math.sin(t*20) for t in times]

    sin_combined = list(map(add, sin_2Hz, sin_20Hz_noise))
    sin_filtered = [filt.do_sample(x) for x in sin_combined]

    # Compare filtered vs original
    delta = [i - j for i, j in zip(sin_2Hz[nTaps/2:-nTaps/2], sin_filtered[nTaps:])]

    if max(delta) > .01:
        print 'Delta (%f) between original and filtered data > .01' % max(delta)
        exit(-1)

    #import matplotlib.pyplot as plt
    #plt.plot(sin_2Hz[nTaps/2:-nTaps/2])
    #plt.plot(sin_filtered[nTaps:])
    #plt.plot(delta)
    #plt.show()



if __name__ == '__main__':
    import sys
    if len(sys.argv) > 1:
        sys.path.insert(0, sys.argv[-1])
    main()
