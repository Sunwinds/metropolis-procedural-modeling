MPM has been tested on Linux and OS X: there is no support for Windows.

# Prerequisites #

  * Qt 4
  * Ruby 1.9 (using Ruby 1.8 will cause silent failures due an implicit dependence in the codebase on an insertion-order preserving hash)
  * GLEW
  * polyglot, treetop

# Setup #
  * Check out the codebase to some directory, and set an environment variable PROC\_MOD\_BASE\_DIR pointing to this location.
  * Add PROC\_MOD\_BASE\_DIR/Common/lib to LD\_LIBRARY\_PATH
  * Build libProcModel via Bootstrap, make, and make install.
  * Build ProcModeler (you do not need to install it)
  * Compile a grammar by running ./Parser Grammars/GrammarName.  This will create a directory in Grammars/Source/ with the shell code for that grammar: you will need to update the render methods.  Once this is done, run the parser a second time.
  * Now, load the ProcModeler, and load the grammar from GrammarGen/Plugins.

# Notes #
  * Most of the included grammars do not have render methods at present.  See the Willow grammar for an example of one that does.
  * You can check the treetop files to see the metagrammar that's defined for writing your own grammars.
  * Only sketch based targeting is included in this release: however, it's easy to replace this targeting function with your own.
  * CUDA support, mesh output, and several other pieces of functionality are not included at present.