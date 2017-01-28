KNOWN ISSUES
	-Breaking blocks on a chunk border will sometimes make the blocks on the adjacent chunk black - this is because of the
	 ordering in which chunks are updated in the list. Since vertex array updating is amortized some chunks still think
	 there are blocks there when there aren't
	-If you stand still long enough, clouds will simply disappear off into the distance and won't regenerate / never get deleted
	-The raycasting to a snow top block (Which is 20% the height of a regular block) still treats it as a regular 1x1x1 block so it may be confusing at first
	-Snow and rain columns will go through buildings built above-ground (Weather won't show underground though!)
	-Player position isn't saved to disk and is hard-coded to a specific value

HOW TO USE
	Keyboard Controls:
		ESDF to move along the XY-Plane.
		MOUSE to look.
		SPACEBAR to move up along the Z-axis.
		C to move down along the Z-axis.
		LEFT-MOUSE to place a block.
		RIGHT-MOUSE to delete a block.
		SHIFT to move faster.
		MOUSE-WHEEL SCROLL to switch between placeable blocks.
		LEFT, RIGHT to switch between placeable blocks
		A to switch camera modes
		G to switch movement modes

DEEP LEARNING
	Organization and writing clean code is absolutely everything. If you're trying to get something working it's okay to be a little hacky, but not having a high
	standard for cleanliness, modularity, and best practices can come to bite you in the butt if you step away and work on something else, or if enough stuff 
	is added to the codebase before you go back to refactor everything and get it looking nice again. There's a fine line between over-engineering the solution and
	trying to make everything fit this intangible sense of "perfect", but the moment the code stops writing itself you must go back and refactor so that you can
	keep your head in the game.
	
	Don't be afraid to throw out entire systems if they didn't work the first time around. Many edge cases pop up as a beginner games programmer that you may not 
	have originally designed for, so there's nothing wrong with going back and refactoring a global singleton weather handler into a chunk-local vector array that instantiates
	weather columns if the column is in a particular biome. It is better to go back and redesign that component or system than it is to keep wading through it and waiting to refactor it,
	because it may get to a point where the system gets so heavy and complex that a refactor of that system may become incredibly complicated. Cut your losses as soon as possible and redesign
	with the new knowledge that you have about the system that you're designing for.