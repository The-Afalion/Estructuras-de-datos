PRACTICE = Practica_4

.PHONY: all clean p4 clean-p4 run-p4 run_e1_normal run_e1_sorted \
        run_e1_100_normal run_e1_100_sorted run_e1_1k_normal \
        run_e1_1k_sorted run_e1_all run_e3

all: p4

p4:
	$(MAKE) -C $(PRACTICE)

clean:
	$(MAKE) -C $(PRACTICE) clean

clean-p4:
	$(MAKE) -C $(PRACTICE) clean

run-p4:
	$(MAKE) -C $(PRACTICE) run_e1_all

run_e1_normal:
	$(MAKE) -C $(PRACTICE) run_e1_normal

run_e1_sorted:
	$(MAKE) -C $(PRACTICE) run_e1_sorted

run_e1_100_normal:
	$(MAKE) -C $(PRACTICE) run_e1_100_normal

run_e1_100_sorted:
	$(MAKE) -C $(PRACTICE) run_e1_100_sorted

run_e1_1k_normal:
	$(MAKE) -C $(PRACTICE) run_e1_1k_normal

run_e1_1k_sorted:
	$(MAKE) -C $(PRACTICE) run_e1_1k_sorted

run_e1_all:
	$(MAKE) -C $(PRACTICE) run_e1_all

run_e3:
	$(MAKE) -C $(PRACTICE) run_e3
