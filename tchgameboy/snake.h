struct snake {
    struct node {
        int x, y;
        node* next;
        node(int x, int y) : x(x), y(y), next(NULL) {};
    };
    int dx, dy;
    node *head, *tail;
    snake(int x, int y) : head(new node(x, y)), tail(head) { dx = 1, dy = 0; }
    void update(node *p, node *pre) {
        if (p->next)
            update(p->next, p);
        if (pre)
            p->x = pre->x,
            p->y = pre->y;
        else
            p->x += dx,
            p->y += dy;
    }
    void turn(int x, int y) {
        dx = x;
        dy = y;
    }
    void add_node() {
        int x = tail->x, y = tail->y;
        update(head, NULL);
        tail->next = new node(x, y);
        tail = tail->next;
    }
};