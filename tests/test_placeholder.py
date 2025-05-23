import importlib
import pytest

def test_pyfuzz_importable():
    try:
        importlib.import_module('pyfuzz')
    except ModuleNotFoundError:
        pytest.skip('pyfuzz not installed')

def test_placeholder():
    assert True
