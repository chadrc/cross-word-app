<?hh
require_once("../src/elements/pages/base-page.hh");
require_once("../src/elements/content.hh");
require_once("../src/elements/cross-word-game.hh");

class :puzzle-page extends :base-page {

  protected function render(): XHPRoot {
    $puzzle_id = "";
    if ($this->path->containsKey(1)) {
      $puzzle_id = $this->path[1];
    }

    $cross_word = DB()->getCrossWord($puzzle_id);

    if ($cross_word !== null) {
      $grid = $cross_word->get_grid();
      $horz_words = Vector {};
      $vert_words = Vector {};
      foreach ($cross_word->get_words() as $word) {
        if ($word->is_horizontal()) {
          $horz_words[] = $word;
        } else {
          $vert_words[] = $word;
        }
      }

      return (
        <content title="Puzzle" components={Vector {"CrossWordGame"}}>
          <x:js-scope>
            <cross-word-game
              horizontal={$horz_words}
              vertical={$vert_words}
              grid={$grid}
              puzzleId={$cross_word->get_id()}/>
          </x:js-scope>
        </content>
      );
    } else {
      return <not-found message="Puzzle could not be found." />;
    }
  }
}
