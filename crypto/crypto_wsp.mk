.PHONY: clean All

All:
	@echo ----------Building project:[ crypto - Debug ]----------
	@"$(MAKE)" -f "crypto.mk"
clean:
	@echo ----------Cleaning project:[ crypto - Debug ]----------
	@"$(MAKE)" -f "crypto.mk" clean
