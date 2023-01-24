.PHONY: clean All

All:
	@echo "----------Building project:[ NoPrintf - Debug ]----------"
	@"$(MAKE)" -f  "NoPrintf.mk"
clean:
	@echo "----------Cleaning project:[ NoPrintf - Debug ]----------"
	@"$(MAKE)" -f  "NoPrintf.mk" clean
