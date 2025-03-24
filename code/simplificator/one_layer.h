#pragma once

#include "ast.h"

// simplify simplifies a node.
// This function only performs one-layer simplification.
// It will return the input node instead of creating a new node, but you shouldn't use the input node anymore beacaue it may be freed.
// It simplifies the node in the following rule:
// 1. 1*node = node*1 = node
// 2. 0*node = node*0 = 0
// 3. 0+node = node+0 = node
// 4. node-0 = node
// 5. ln(1) = 0
// 6. pow(1, node) = 1
// 7. pow(node, 0) = 1
// 8. pow(node, 1) = node
// 9. 0/node = 0
// 10. node/1 = node
// 11. 0-node = -node
Node* simple_simplify(Node *node);