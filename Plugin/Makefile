SUBSUBDIRS = SCPIUtils SCPITools SCPISrc
.PHONY: print subdirs $(SUBSUBDIRS) clean all test

subdirs: print $(SUBSUBDIRS)
$(SUBSUBDIRS):
	$(MAKE) -C $@
all:
	$(MAKE) -C libscpi
	$(MAKE) -C examples

clean:
	$(MAKE) clean -C libscpi
	$(MAKE) clean -C examples
	$(MAKE) clean -C SCPIUtils
	$(MAKE) clean -C SCPITools
	$(MAKE) clean -C SCPISrc
	(rm -f SCPIlib/* SCPIbin/*)

test:
	$(MAKE) test -C libscpi
	
src: HWDescription HWInterface Utils System tools
System: HWDescription HWInterface Utils