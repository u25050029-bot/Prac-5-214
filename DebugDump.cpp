#include "DebugDump.h"

#include <cstddef>
#include <string>

#include "AreaComponent.h"
#include "Log.h"

void DebugDump::tree(AreaComponent* node) {
    Log::line("debug", "tree dump, " + std::to_string(countNodes(node)) + " node(s)");
    treeAt(node, 0);
}

void DebugDump::treeAt(AreaComponent* node, int indent) {
    std::string pad;
    for (int i = 0; i < indent; ++i) {
        pad += "  ";
    }
    Log::line("Area", pad + node->getKind() + " " + node->getName() + ": " + node->statusText());
    for (std::size_t i = 0; i < node->childCount(); ++i) {
        treeAt(node->childAt(i), indent + 1);
    }
}

void DebugDump::doors(AreaComponent* node) {
    if (node->childCount() == 0) {
        Log::line("debug", "door " + node->getName() + " = " + node->statusText());
        return;
    }
    for (std::size_t i = 0; i < node->childCount(); ++i) {
        doors(node->childAt(i));
    }
}

int DebugDump::countNodes(AreaComponent* node) {
    int total = 1;
    for (std::size_t i = 0; i < node->childCount(); ++i) {
        total += countNodes(node->childAt(i));
    }
    return total;
}
