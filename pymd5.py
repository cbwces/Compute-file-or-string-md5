import hashlib
import sys

# start = time.time()
md_hash_dict = dict()
current_hasher = set()
with open(sys.argv[1], 'r') as f:
    for line in f.readlines():
        l = line.strip()
        with open(l, 'rb') as g:
            md5 = hashlib.md5(g.read()).hexdigest()
            if md5 in md_hash_dict:
                current_hasher.add(l)
                current_hasher.add(md_hash_dict[md5])
            md_hash_dict[md5] = l
for line in current_hasher:
    print(line)
# print(time.time()-start)
        

