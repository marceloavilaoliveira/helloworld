#!/usr/bin/python

# This is a test
import getopt
import sys
from LoggingEngine.logger import *
from Handlers.dlnamanager import DlnaManager

def usage():
    print ""
    print " -h/--help       Usage"
    print " -s/--server     Redis server address (default=localhost)"
    print " -p/--port       Redis server port (default=6379)"
    print ""

def main():
    # This is the main
    logfile = DEFAULT_LOGFILE
    redis_host = DEFAULT_REDIS_HOST
    redis_port = DEFAULT_REDIS_PORT
    LOGGER.info("Starting DLNA Management Deamon.")
    LOGGER.info("Logging are being written in %s. ", logfile)

    try:
        opts, args = getopt.getopt(sys.argv[1:], "hs:p:f:", ["help", "server=", "port=", "file="])
    except getopt.GetoptError as err:
        # print help information and exit:
        print err  # will print something like "option -a not recognized"
        usage()
        sys.exit(2)

    # For those about to rock
    for option, aux in opts:
        if option in ("-h", "--help"):
            # Show Usage
            usage()
            sys.exit()
        elif option in ("-s", "--server"):
            redis_host = aux
        elif option in ("-p", "--port"):
            redis_port = aux

    LOGGER.info("Using Redis Server at tcp://%s:%s" % (redis_host, redis_port))

    # The main thread listens for configuration events
    dlna_manager = DlnaManager(redis_host, redis_port)
    dlna_manager.run()

if __name__ == '__main__':
    main()
