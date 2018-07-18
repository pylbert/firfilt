#! /usr/bin/env python


def main(filename_raw_in, filename_filtered_out):
    import fir
    import tempfile

    filt = fir.Filter_LP(51, 1000, 7)

    fdout = tempfile.NamedTemporaryFile(delete = False)

    with open(filename_raw_in) as f:
        for line in f:
            try:
                raw = float(line.strip())
                fdout.write('%0.08f\n' % filt.do_sample(raw))
            except:
                pass

    fdout.flush()
    # Compare the generated file with the golden filtered file
    import filecmp
    if not filecmp.cmp(filename_filtered_out, fdout.name):
        print 'Files not equal: ', filename_filtered_out, fdout.name
        exit(-1)

    # Delete the file on success
    try:
        import os
        os.remove(fdout.name)
    except OSError:
        pass

if __name__ == '__main__':
    import sys
    if len(sys.argv) != 4:
        print 'usage: %s /path/to/fir_module /path/to/filter_in_file /path/to/filter_out_golden' % sys.argv[0]
        exit(-1)

    # Add the module file path to path
    sys.path.insert(0, sys.argv[1])
    main(sys.argv[2], sys.argv[3])
