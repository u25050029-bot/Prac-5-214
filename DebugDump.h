#ifndef CAMPUSGUARD_DEBUGDUMP_H
#define CAMPUSGUARD_DEBUGDUMP_H

class AreaComponent;

class DebugDump {
public:
    static void tree(AreaComponent* node);
    static void doors(AreaComponent* node);
    static int countNodes(AreaComponent* node);

private:
    static void treeAt(AreaComponent* node, int indent);
};

#endif
