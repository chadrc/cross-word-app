class CrossWordGame extends React.Component {
  constructor(props) {
    super(props);
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

  render() {
    let rows = []
    for (let y=this.props.limits.maxY; y>=this.props.limits.minY; y--) {
      let cells = [];
      for (let x=this.props.limits.minX; x<=this.props.limits.maxX; x++) {
        let cell = this.props.grid[y][x];
        cells.push(
          <td key={`${x},${y}`} className={cell ? "" : "empty"}>
            {cell && cell.orientation ?
              <sup>{this.wordIndex(cell.wordIndex, cell.orientation)}</sup>
            : ""}
            {cell ? <input type="text" maxLength="1" /> : ""}
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
        <section className="cross-word">
          <table>
            <tbody>
              {rows}
            </tbody>
          </table>
        </section>
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
      </section>
    );
  }
}

window.CrossWordGame = CrossWordGame;
