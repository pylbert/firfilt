#! /usr/bin/env python

import fir

def main():
    filt = fir.Filter_LP(51, 1000, 10)

if __name__ == '__main__':
    main()
