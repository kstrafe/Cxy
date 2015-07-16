import re

def prependTabEachLine(string, amount=1):
    """Prepends a tab to each line in the string"""
    return re.sub(r'\n', r'\n' + '    ' * amount, '    ' * amount + string)
