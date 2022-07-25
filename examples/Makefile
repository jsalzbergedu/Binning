.PHONY: all
all: compile

pequin_experiments/pepper/apps/bfs.c: bfs.c
	ln -sf $(realpath bfs.c) $(realpath pequin_experiments/pepper/apps)/bfs.c

pequin_experiments/pepper/input_generation/bfs_v_inp_gen.h: bfs_v_inp_gen.h
	ln -sf $(realpath bfs_v_inp_gen.h) $(realpath pequin_experiments/pepper/input_generation)/bfs_v_inp_gen.h

pequin_experiments/pepper/apps/bfs4.c: bfs4.c
	ln -sf $(realpath bfs4.c) $(realpath pequin_experiments/pepper/apps)/bfs4.c

pequin_experiments/pepper/input_generation/bfs4_v_inp_gen.h: bfs4_v_inp_gen.h
	ln -sf $(realpath bfs4_v_inp_gen.h) $(realpath pequin_experiments/pepper/input_generation)/bfs4_v_inp_gen.h

.PHONY: input_generation
input_generation: pequin_experiments/pepper/apps/bfs_v_inp_gen.h pequin_experiments/pepper/apps/bfs4_v_inp_gen.h

.PHONY: apps
apps: pequin_experiments/pepper/apps/bfs.c pequin_experiments/pepper/apps/bfs4.c

# Could be improved by listing all the files that are generated
pequin_experiments/pepper/bin/pepper_verifier_bfs4 pequin_experiments/pepper/bin/pepper_prover_bfs4: pequin_experiments/pepper/apps/bfs4.c pequin_experiments/pepper/input_generation/bfs4_v_inp_gen.h
	cd pequin_experiments/pepper; \
	./pepper_compile_and_setup_V.sh bfs4 bfs4.vkey bfs4.pkey; \
	./pepper_compile_and_setup_P.sh bfs4

pequin_experiments/pepper/bin/pepper_verifier_bfs pequin_experiments/pepper/bin/pepper_prover_bfs: pequin_experiments/pepper/apps/bfs.c pequin_experiments/pepper/input_generation/bfs_v_inp_gen.h
	cd pequin_experiments/pepper; \
	./pepper_compile_and_setup_V.sh bfs bfs.vkey bfs.pkey; \
	./pepper_compile_and_setup_P.sh bfs

.PHONY: compile
compile: pequin_experiments/pepper/bin/pepper_verifier_bfs4 pequin_experiments/pepper/bin/pepper_prover_bfs4 pequin_experiments/pepper/bin/pepper_verifier_bfs pequin_experiments/pepper/bin/pepper_prover_bfs

.PHONY: run
run: pequin_experiments/pepper/bin/pepper_verifier_bfs4 pequin_experiments/pepper/bin/pepper_prover_bfs4 pequin_experiments/pepper/bin/pepper_verifier_bfs pequin_experiments/pepper/bin/pepper_prover_bfs
	echo "Running BFS4 (echoing BFS path)"
	cd pequin_experiments/pepper; \
	bin/pepper_verifier_bfs4 gen_input bfs4.inputs; \
	bin/pepper_prover_bfs4 prove bfs4.pkey bfs4.inputs bfs4.outputs bfs4.proof; \
	bin/pepper_verifier_bfs4 verify bfs4.vkey bfs4.inputs bfs4.outputs  bfs4.proof
	echo "Running BFS (silent; faster)"
	cd pequin_experiments/pepper; \
	bin/pepper_verifier_bfs gen_input bfs.inputs; \
	bin/pepper_prover_bfs prove bfs.pkey bfs.inputs bfs.outputs bfs.proof; \
	bin/pepper_verifier_bfs verify bfs.vkey bfs.inputs bfs.outputs  bfs.proof

.PHONY: clean
clean:
	rm -rf pequin_experiments/pepper/apps/bfs4.c
	rm -rf pequin_experiments/pepper/input_generation/bfs4_v_inp_gen.h
	rm -rf pequin_experiments/pepper/apps/bfs.c
	rm -rf pequin_experiments/pepper/input_generation/bfs_v_inp_gen.h
