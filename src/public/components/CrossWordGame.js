class CrossWordGame extends React.Component {
  constructor(props) {
    super(props);
    this.state = this.defaultState;
  }

  get defaultState() {
    let answerGrid = {};
    let solved = false;
    if (localStorage[this.props.puzzleId]) {
      let store = JSON.parse(localStorage[this.props.puzzleId]);
      answerGrid = store.answers;
      solved = store.solved === true;
    } else {
      for (let y=this.props.limits.minY; y <= this.props.limits.maxY; y++) {
        answerGrid[y] = {};
      }
      localStorage[this.props.puzzleId] = JSON.stringify({
        answers: answerGrid,
        solved: false
      });
    }
    return {
      forceFocus: null,
      focused: null,
      overflow: "",
      answerGrid: answerGrid,
      solved: solved,
      answerAudits: null
    }
  }

  storeState() {
    localStorage[this.props.puzzleId] = JSON.stringify({
      answers: this.state.answerGrid,
      solved: this.state.solved
    });
  }

  wordIndex(i, orientation) {
    if (!orientation) {
      return NaN;
    }
    if (orientation !== "Horizontal" && orientation !== "Vertical") {
      throw Error("Invalid orientation:" + orientation);
    }
    return orientation === "Horizontal" ? i * 2 + 1 : i * 2 + 2;
  }

  onKeyDown(e) {
    if (e.key === "Tab") {
      let cur = this.state.focused;
      if (!cur) {
        return;
      }

      e.preventDefault();
      e.stopPropagation();

      let nextCell = this.getNextCell(cur.x, cur.y, e.shiftKey ? -1 : 1, e.shiftKey ? 1 : -1)

      this.setState({
        forceFocus: nextCell.next,
        tabDirection: nextCell.direction
      });
    }
  }

  getNextCell(x, y, dirX, dirY) {
    let eastWest = {x: x + dirX, y: y, cell: null};
    eastWest.cell = this.props.grid[eastWest.y][eastWest.x];
    let northSouth = {x: x, y: y + dirY, cell: null};
    northSouth.cell = this.props.grid[northSouth.y] ? this.props.grid[northSouth.y][northSouth.x] : null;
    let next = null;
    if (eastWest.cell && northSouth.cell) {
      next = this.state.tabDirection === "east-west" ? eastWest : northSouth;
    } else {
      next = eastWest.cell ? eastWest : northSouth;
    }

    let dir = "";
    if (next) {
      if (next === eastWest) {
        dir = "east-west";
      } else {
        dir = "north-south";
      }
    }

    return {
      next: next,
      direction: dir
    }
  }

  cellOverflow(x, y, l) {
    let nextCell = this.getNextCell(x, y, 1, -1);
    this.setState({
      forceFocus: nextCell.next,
      tabDirection: nextCell.direction
    });

    this.cellChanged(nextCell.next.x, nextCell.next.y, l);
  }

  cellChanged(x, y, v) {
    this.state.answerGrid[y][x] = v;
    if (this.state.answerAudits) {
      this.state.answerAudits[y][x] = null;
    }
    this.setState({
      answerGrid: this.state.answerGrid,
      answerAudits: this.state.answerAudits
    }, () => {
      this.storeState();
    });
  }

  cellFocused(x, y) {
    this.setState({
      focused: {x: x, y: y},
      forceFocus: null,
      overflow: ""
    })
  }

  forceFocusCell(x, y) {
    return this.state.forceFocus && this.state.forceFocus.x === x && this.state.forceFocus.y === y;
  }

  cellStats(x, y) {
    return this.state.answerAudits ? this.state.answerAudits[y][x] : null;
  }

  cellClass(x, y) {
    let cell = this.props.grid[y][x];
    let stats = this.cellStats(x, y);
    let classes = [];
    if (!cell) {
      classes.push("empty");
    }

    if (stats) {
      if (!stats.correct || stats.missing) {
        classes.push("incorrect");
      }
    }

    return classes.join(" ");
  }

  reset(e) {
    e.preventDefault();
    e.stopPropagation();
    console.log("reset");
    localStorage[this.props.puzzleId] = "";
    this.setState(this.defaultState)
  }

  submit(e) {
    console.log("solve");
    e.preventDefault();
    e.stopPropagation();
    fetch("/puzzle/solve", {
      method: "POST",
      body: JSON.stringify({
        puzzleId: this.props.puzzleId,
        answers: this.state.answerGrid
      }),
      headers: new Headers({
        'Content-Type': 'application/json'
      })
    }).then((response) => {
      return response.json();
    }).then((obj) => {
      this.setState({
        solved: obj.solved,
        answerAudits: obj.answers
      }, () => {
        this.storeState();
      });
    });
  }

  render() {
    let rows = []
    for (let y=this.props.limits.maxY; y>=this.props.limits.minY; y--) {
      let cells = [];
      for (let x=this.props.limits.minX; x<=this.props.limits.maxX; x++) {
        let cell = this.props.grid[y][x];
        cells.push(
          <td key={`${x},${y}`} className={this.cellClass(x, y)}>
            {cell && cell.orientation ?
              <sup>{this.wordIndex(cell.wordIndex, cell.orientation)}</sup>
            : ""}
            {cell ?
              <CellInput  x={x} y={y}
                          stats={this.cellStats(x, y)}
                          forceFocus={this.forceFocusCell(x, y)}
                          onFocus={() => this.cellFocused(x, y)}
                          onOverflow={(l) => this.cellOverflow(x, y, l)}
                          onChange={(v) => this.cellChanged(x, y, v)}
                          value={this.state.answerGrid[y][x] || ""}
                          disabled={this.state.solved}
              />
            : ""}
          </td>
        );
      }
      rows.push(
        <tr key={y}>{cells}</tr>
      );
    }
    return (
      <section>
        <h1>Solve</h1>
        <form onSubmit={(e) => this.submit(e)} className={this.state.solved ? "solved" : ""}>
          <section className="cross-word">
              <table onKeyDown={(e) => this.onKeyDown(e)}>
                <tbody>
                  {rows}
                </tbody>
              </table>
          </section>
          {this.state.solved ? (
            <section className="puzzle-status">
              <h2>Solved!</h2>
            </section>
          ) : ""}
          <section className="word-hints">
            <ul>
              <li>Horizontal</li>
              {this.props.horizontal.map((item, index) => {
                return (
                  <li key={`horizontal${index}`}>{this.wordIndex(index, "Horizontal")}. {item}</li>
                );
              })}
            </ul>
            <ul>
              <li>Vertical</li>
              {this.props.vertical.map((item, index) => {
                return (
                  <li key={`vertical${index}`}>{this.wordIndex(index, "Vertical")}. {item}</li>
                );
              })}
            </ul>
          </section>
          {this.state.solved ? (
            <button type="button" onClick={(e) => this.reset(e)}>Reset</button>
          ) : (
            <button type="submit">Submit</button>
          )}
        </form>
      </section>
    );
  }
}

window.CrossWordGame = CrossWordGame;

class CellInput extends React.Component {
  constructor(props) {
    super(props);
    this.state = {
    }
  }

  raiseOnChange(value) {
    let letter = value.substr(0, 1);
    let overflow = value.substr(1);

    if (letter !== this.props.value) {
      if (this.props.onChange) {
        this.props.onChange(value);
      }
    } else if (overflow && this.props.onOverflow) {
      this.props.onOverflow(overflow);
    }
  }

  raiseOnFocus() {
    if (this.props.onFocus) {
      this.props.onFocus();
    }
  }

  raiseOnBlur() {
    if (this.props.onBlur) {
      this.props.onBlur();
    }
  }

  render() {
    return (
      <input  type="text"
              tabIndex={-1}
              ref={(input) => this.input = input}
              value={this.props.value}
              onClick={(e) => this.input.select()}
              onFocus={(e) => this.raiseOnFocus()}
              onBlur={(e) => this.raiseOnBlur()}
              onChange={(e) => this.raiseOnChange(e.target.value)}
              disabled={this.props.disabled}
      />
    );
  }

  componentDidUpdate() {
    if (this.props.forceFocus) {
      this.input.focus();
    }
  }
}
