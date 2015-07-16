def toNamespaces(*namespaces_):
    head = ''
    tail = ''

    for namespace_ in namespaces_:
        head += 'namespace ' + namespace_ + ' { '

    tail = len(namespaces_) * '}'

    return head, tail
