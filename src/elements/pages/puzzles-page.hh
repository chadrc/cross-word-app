<?hh
require_once("../src/elements/content.hh");
require_once("../src/elements/pages/base-page.hh");

class :puzzles-page extends :base-page {
  protected function render(): XHPRoot {
    $count = 10;
    $page = 0;

    if ($this->get_data->containsKey("count")) {
      $count = (int) $this->get_data["count"];
    }

    if ($this->get_data->containsKey("page")) {
      $page = (int) $this->get_data["page"];
    }

    $table = <table></table>;

    $table->appendChild(
      <tr>
        <th>Word Count</th>
        <th>Size</th>
        <th>Created</th>
        <th>Play</th>
      </tr>
    );

    $crosswords = DB()->getCrossWords($count, $page);

    foreach ($crosswords->get_items() as $value) {
      $limits = $value->get_grid()->get_limits();
      $width = $limits->max_x() - $limits->min_x();
      $height = $limits->max_y() - $limits->min_y();
      $table->appendChild(
        <tr>
          <td>{$value->get_words()->count()}</td>
          <td>({$width}, {$height})</td>
          <td>{$value->get_created()->format("Y-m-d")}</td>
          <td><a href={"puzzle/" . $value->get_id()}>Play</a></td>
        </tr>
      );
    }

    $total_pages = $crosswords->get_total() / $count;
    $nav_route = "puzzles?" . ($count != 10 ? "count=" . $count : "");

    $prev_link = <a>Prev</a>;
    if ($page > 0) {
      $prev_link->setAttribute("href", $nav_route . "&page=" . ($page - 1));
    }

    $next_link = <a>Next</a>;
    if ($page < ($total_pages - 1)) {
      $next_link->setAttribute("href", $nav_route . "&page=" . ($page + 1));
    }

    return (
      <content title="Puzzles">
        <h1>Pick a puzzle</h1>
        {$table}
        <nav>
          {$prev_link}
          {$next_link}
        </nav>
      </content>
    );
  }
}
