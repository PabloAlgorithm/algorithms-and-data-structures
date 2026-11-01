"""Дополнительные проверки трёх программ: python test_cli.py <каталог exe>.
В Visual Studio используются имена ArrayProgram/CStringProgram/FileProgram.
"""
from pathlib import Path
import subprocess
import sys
import tempfile

bin_dir = Path(sys.argv[1]).resolve()
count = 0

def exe(normal, short):
    path = bin_dir / (normal + '.exe')
    return path if path.exists() else bin_dir / (short + '.exe')

def run(path, text=b'', args=(), cwd=None):
    return subprocess.run([str(path), *args], input=text, capture_output=True, cwd=cwd)

for normal, short in [('ArrayProgram', 'array'), ('CStringProgram', 'cstring')]:
    path = exe(normal, short)
    for original, expected in [('', ''), ('Моя машина тут', 'Моя  тут'),
                                ('elephant planet', 'elephant planet'),
                                ('книга planet', 'книга '), ('Ёлочка', 'Ёлочка'),
                                ('b' * 1023, '')]:
        result = run(path, (original + '\n').encode())
        assert result.returncode == 0, result.stderr
        assert ('Результат: [' + expected + ']').encode() in result.stdout
        count += 1
    result = run(path, b'b' * 1024 + b'\n')
    assert result.returncode != 0 and result.stderr
    count += 1

path = exe('FileProgram', 'file')
with tempfile.TemporaryDirectory(prefix='lab01_cli_') as d:
    work = Path(d)
    (work / 'input.txt').write_text('Моя машина тут\n', encoding='utf-8')
    result = run(path, cwd=work)
    assert result.returncode == 0, result.stderr
    assert (work / 'output.txt').read_text(encoding='utf-8') == 'Моя  тут'
    count += 1
    (work / 'custom.txt').write_text('planet', encoding='utf-8')
    result = run(path, args=('custom.txt', 'custom-result.txt'), cwd=work)
    assert result.returncode == 0
    assert (work / 'custom-result.txt').read_bytes() == b''
    count += 1
    for args in [('missing.txt', 'output.txt'), ('input.txt', 'input.txt'), ('one-argument',)]:
        result = run(path, args=args, cwd=work)
        assert result.returncode != 0 and result.stderr
        count += 1
print(f'CLI checks passed: {count}')
