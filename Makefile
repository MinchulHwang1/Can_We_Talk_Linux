# Directories for each application
DIRS = chat-client chat-server Common

# 'all' target will build all applications
all:
	@for dir in $(DIRS); do \
	echo "Making all in $$dir..."; \
	$(MAKE) -C $$dir; \
	done

# 'clean' target will clean all applications
clean:
	@for dir in $(DIRS); do \
	echo "Cleaning in $$dir..."; \
	$(MAKE) -C $$dir clean; \
	done

.PHONY: all clean

