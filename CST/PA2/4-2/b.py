class SegmentTreeNode:
    def __init__(self, l, r, k, lazy=0, left=None, right=None):
        self.l = l
        self.r = r
        self.k = k
        self.lazy = lazy
        self.left = left
        self.right = right

    def __str__(self):
        return str(self.__dict__)


def query(node, i, j):
    normalize(node)
    if i > j or not node or i > node.r or j < node.l:
        return 0
    if node.l >= i and node.r <= j:
        return node.k
    return query(node.left, i, j) + query(node.right, i, j)


def update(node, i, j, val=1):
    normalize(node)
    if i > j or not node or i > node.r or j < node.l:
        return
    if node.l >= i and node.r <= j:
        node.lazy = val
        normalize(node)
        return
    update(node.left, i, j, val)
    update(node.right, i, j, val)
    node.k = node.left.k + node.right.k


def normalize(node):
    if node.lazy > 0:
        node.k += node.lazy * (node.r - node.l + 1)
    if node.l < node.r:
        if not node.left or not node.right:
            mid = (node.l + node.r) // 2
            node.left = SegmentTreeNode(node.l, mid, 0)
            node.right = SegmentTreeNode(mid + 1, node.r, 0)
        node.left.lazy += node.lazy
        node.right.lazy += node.lazy

    node.lazy = 0


S = input()
n = int(S.split(' ')[0])
m = int(S.split(' ')[1])

root = SegmentTreeNode(1, n, 0)

for i in range(0, m):
    S = input()
    c = S.split(' ')[0]
    if c == 'H':
        l = int(S.split(' ')[1])
        r = int(S.split(' ')[2])
        update(root, l, r)
    else:
        merge = 0
        l = int(S.split(' ')[1])
        r = int(S.split(' ')[2])
        print(query(root, l, r))
