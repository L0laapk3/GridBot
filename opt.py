from hyperopt import hp, fmin, tpe
import subprocess

scale = [100, 3.5, 1.5, 1000000, 1.5]
def run(args):
	args = list(args)
	for i in range(5):
		args[i] *= scale[i]
	args = list(map(lambda s: str(s), args))
	args.insert(0, "x64/Release/GridBot.exe")
	return -int(subprocess.Popen(args, stdout=subprocess.PIPE).communicate()[0])

space = [
	hp.lognormal('mergeWeight', 0, 1),
	hp.lognormal('monotonicityPower', 0, 1),
	hp.lognormal('monotonicityWeight', 0, 1),
	hp.lognormal('valuesWeight', 0, 1),
	hp.lognormal('multiple3Power', 0, 10)
]

best = fmin(run,
    space=space,
    algo=tpe.suggest,
    max_evals=5000)

print(best)