using Microsoft.EntityFrameworkCore.Migrations;

#nullable disable

namespace WebTemplate.Migrations
{
    /// <inheritdoc />
    public partial class v1 : Migration
    {
        /// <inheritdoc />
        protected override void Up(MigrationBuilder migrationBuilder)
        {
            migrationBuilder.CreateTable(
                name: "Dimenzije",
                columns: table => new
                {
                    ID = table.Column<int>(type: "int", nullable: false)
                        .Annotation("SqlServer:Identity", "1, 1"),
                    Visina = table.Column<int>(type: "int", nullable: false),
                    Sirina = table.Column<int>(type: "int", nullable: false)
                },
                constraints: table =>
                {
                    table.PrimaryKey("PK_Dimenzije", x => x.ID);
                });

            migrationBuilder.CreateTable(
                name: "Papiri",
                columns: table => new
                {
                    ID = table.Column<int>(type: "int", nullable: false)
                        .Annotation("SqlServer:Identity", "1, 1"),
                    Naziv = table.Column<string>(type: "nvarchar(max)", nullable: false)
                },
                constraints: table =>
                {
                    table.PrimaryKey("PK_Papiri", x => x.ID);
                });

            migrationBuilder.CreateTable(
                name: "Prodavnice",
                columns: table => new
                {
                    ID = table.Column<int>(type: "int", nullable: false)
                        .Annotation("SqlServer:Identity", "1, 1"),
                    Naziv = table.Column<string>(type: "nvarchar(max)", nullable: false),
                    DnevnaZarada = table.Column<int>(type: "int", nullable: false)
                },
                constraints: table =>
                {
                    table.PrimaryKey("PK_Prodavnice", x => x.ID);
                });

            migrationBuilder.CreateTable(
                name: "Ramovi",
                columns: table => new
                {
                    ID = table.Column<int>(type: "int", nullable: false)
                        .Annotation("SqlServer:Identity", "1, 1"),
                    BrRamova = table.Column<int>(type: "int", nullable: false),
                    Materijal = table.Column<string>(type: "nvarchar(max)", nullable: false),
                    DimenzijaRamaID = table.Column<int>(type: "int", nullable: true)
                },
                constraints: table =>
                {
                    table.PrimaryKey("PK_Ramovi", x => x.ID);
                    table.ForeignKey(
                        name: "FK_Ramovi_Dimenzije_DimenzijaRamaID",
                        column: x => x.DimenzijaRamaID,
                        principalTable: "Dimenzije",
                        principalColumn: "ID");
                });

            migrationBuilder.CreateTable(
                name: "Fotografije",
                columns: table => new
                {
                    ID = table.Column<int>(type: "int", nullable: false)
                        .Annotation("SqlServer:Identity", "1, 1"),
                    BrFotografija = table.Column<int>(type: "int", nullable: false),
                    SlikaPath = table.Column<string>(type: "nvarchar(max)", nullable: false),
                    Naziv = table.Column<string>(type: "nvarchar(max)", nullable: false),
                    ProdavnicaID = table.Column<int>(type: "int", nullable: true),
                    PapirID = table.Column<int>(type: "int", nullable: true),
                    DimenzijaID = table.Column<int>(type: "int", nullable: true),
                    RamID = table.Column<int>(type: "int", nullable: true)
                },
                constraints: table =>
                {
                    table.PrimaryKey("PK_Fotografije", x => x.ID);
                    table.ForeignKey(
                        name: "FK_Fotografije_Dimenzije_DimenzijaID",
                        column: x => x.DimenzijaID,
                        principalTable: "Dimenzije",
                        principalColumn: "ID");
                    table.ForeignKey(
                        name: "FK_Fotografije_Papiri_PapirID",
                        column: x => x.PapirID,
                        principalTable: "Papiri",
                        principalColumn: "ID");
                    table.ForeignKey(
                        name: "FK_Fotografije_Prodavnice_ProdavnicaID",
                        column: x => x.ProdavnicaID,
                        principalTable: "Prodavnice",
                        principalColumn: "ID");
                    table.ForeignKey(
                        name: "FK_Fotografije_Ramovi_RamID",
                        column: x => x.RamID,
                        principalTable: "Ramovi",
                        principalColumn: "ID");
                });

            migrationBuilder.CreateIndex(
                name: "IX_Fotografije_DimenzijaID",
                table: "Fotografije",
                column: "DimenzijaID");

            migrationBuilder.CreateIndex(
                name: "IX_Fotografije_PapirID",
                table: "Fotografije",
                column: "PapirID");

            migrationBuilder.CreateIndex(
                name: "IX_Fotografije_ProdavnicaID",
                table: "Fotografije",
                column: "ProdavnicaID");

            migrationBuilder.CreateIndex(
                name: "IX_Fotografije_RamID",
                table: "Fotografije",
                column: "RamID");

            migrationBuilder.CreateIndex(
                name: "IX_Ramovi_DimenzijaRamaID",
                table: "Ramovi",
                column: "DimenzijaRamaID");
        }

        /// <inheritdoc />
        protected override void Down(MigrationBuilder migrationBuilder)
        {
            migrationBuilder.DropTable(
                name: "Fotografije");

            migrationBuilder.DropTable(
                name: "Papiri");

            migrationBuilder.DropTable(
                name: "Prodavnice");

            migrationBuilder.DropTable(
                name: "Ramovi");

            migrationBuilder.DropTable(
                name: "Dimenzije");
        }
    }
}
