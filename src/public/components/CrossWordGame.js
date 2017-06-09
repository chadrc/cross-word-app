class CrossWordGame extends React.Component {
  constructor(props) {
    super(props);
  }

  render() {
    let rows = []
    for (let y=this.props.limits.maxY; y>=this.props.limits.minY; y--) {
      let cells = [];
      for (let x=this.props.limits.minX; x<=this.props.limits.maxX; x++) {
        let cell = this.props.grid[y][x];
        cells.push(
          <td key={`${x},${y}`} className={cell ? "" : "empty"}></td>
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
                <li key={`horizontal${index}`}>{index * 2}. {item}</li>
              );
            })}
          </ul>
          <ul>
            <li>Vertical</li>
            {this.props.vertical.map((item, index) => {
              return (
                <li key={`vertical${index}`}>{index * 2 + 1}. {item}</li>
              );
            })}
          </ul>
        </section>
      </section>
    );
  }
}

window.CrossWordGame = CrossWordGame;
