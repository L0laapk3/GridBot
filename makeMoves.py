import sys


chainLengths = [1, 2, 4, 8]
for n in range(0, 15):
	val = 3 * 2**n
	chainLengths.append(val)


MAXLENGTH = int(sys.argv[1]) if len(sys.argv) > 1 else 6
chainLengths.sort()
chainLengths = tuple(filter(lambda x: x > 1 and x <= MAXLENGTH, chainLengths))





def makeChains(chain, left):
	if left == 0:
		return [chain]
	
	result = []
	for m in ((0, 1), (1, 0), (0, -1), (-1, 0)):
		nextMove = ((chain[-1][0] + m[0], chain[-1][1] + m[1]))
		if nextMove not in chain:
			for e in makeChains(chain + [nextMove], left - 1):
				result.append(e)
	return result
	
i = 0
chains = []
for chainLength in chainLengths:
	for chain in makeChains([(0, 0)], chainLength - 1):
		X = tuple(map(lambda e: e[0], chain))
		Y = tuple(map(lambda e: e[1], chain))
		if max(X) - min(X) < 5 and max(Y) - min(Y) < 5:

			reverse = list(chain)
			reverse.reverse()
			reverse = list(map(lambda x: (x[0] - chain[-1][0], x[1] - chain[-1][1]), reverse))
			if reverse not in chains:
				chains.append(chain)


tree = {}
moves = []
for chain in chains:

	i += 1

	curr = tree
	last = chain[0]
	s = ""
	for entry in chain[1:]:
		if entry[0] - last[0] == 1:
			now = "r"
		elif entry[0] - last[0] == -1:
			now = "l"
		elif entry[1] - last[1] == 1:
			now = "u"
		elif entry[1] - last[1] == -1:
			now = "d"
		else:
			print("wtf", entry, last)
		s += now
		last = entry
		if now not in curr:
			curr[now] = {}
		curr = curr[now]
	moves.append(s)
	

for move in moves:
	print(move)
	node = tree
	for d in move:
		node = node[d]
	node[0] = 1

print(i)
print(tree)

def buildTree(elOrig, intend = 0):
	tab = '  '
	el = elOrig.copy()
	if 0 in elOrig:
		del el[0]
	if len(el.keys()) > 0:
		s = ""
		if intend > 0:
			s += "new Node"
		s += "{"
		multiline = any(map(lambda d: d != 1 and any(map(lambda x: x in d, "udrl")), el.values()))
		children = list(map(lambda d: buildTree(el[d], intend + 1) if d in el else ("nullptr" if multiline else "nullptr   "), "udrl"))
		if 0 not in elOrig:
			children.append("false")
		if multiline:
			children = map(lambda d: "\n" + (intend+1) * tab + d, children)
		else:
			s += " "
		s += ", ".join(children)
		if multiline:
			s += "\n" + intend * tab
		else:
			s += " "
		s += "}"
		return s
	else:
		return "new Node{}"

with open('MovesTree.cpp', "w") as f:
	f.write("#include \"MovesTree.h\"\n\n")
	f.write("const Node ALLMOVES" + buildTree(tree) + ";\n")

