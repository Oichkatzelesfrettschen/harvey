import os
import sys
sys.path.insert(0, os.path.abspath('..'))

extensions = [
    'breathe',
    'sphinx_rtd_theme',
]

breathe_projects = {'harvey': '../build/docs/xml'}
breathe_default_project = 'harvey'

html_theme = 'sphinx_rtd_theme'
master_doc = 'index'
project = 'Harvey Utilities'
