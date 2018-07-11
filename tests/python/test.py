#! /usr/bin/env python

def main():
    import fir
    filt = fir.Filter_LP(51, 1000, 10)

if __name__ == '__main__':
    import sys
    if len(sys.argv) > 1:
        sys.path.insert(0, sys.argv[-1])
    main()
