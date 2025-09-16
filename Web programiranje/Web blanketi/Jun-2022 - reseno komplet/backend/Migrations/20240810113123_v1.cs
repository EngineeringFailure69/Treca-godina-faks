using System;
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
                name: "Marke",
                columns: table => new
                {
                    ID = table.Column<int>(type: "int", nullable: false)
                        .Annotation("SqlServer:Identity", "1, 1"),
                    Naziv = table.Column<string>(type: "nvarchar(max)", nullable: false)
                },
                constraints: table =>
                {
                    table.PrimaryKey("PK_Marke", x => x.ID);
                });

            migrationBuilder.CreateTable(
                name: "Modeli",
                columns: table => new
                {
                    ID = table.Column<int>(type: "int", nullable: false)
                        .Annotation("SqlServer:Identity", "1, 1"),
                    Naziv = table.Column<string>(type: "nvarchar(max)", nullable: false),
                    MarkaID = table.Column<int>(type: "int", nullable: true)
                },
                constraints: table =>
                {
                    table.PrimaryKey("PK_Modeli", x => x.ID);
                    table.ForeignKey(
                        name: "FK_Modeli_Marke_MarkaID",
                        column: x => x.MarkaID,
                        principalTable: "Marke",
                        principalColumn: "ID");
                });

            migrationBuilder.CreateTable(
                name: "Boje",
                columns: table => new
                {
                    ID = table.Column<int>(type: "int", nullable: false)
                        .Annotation("SqlServer:Identity", "1, 1"),
                    Naziv = table.Column<string>(type: "nvarchar(max)", nullable: false),
                    ModelID = table.Column<int>(type: "int", nullable: true)
                },
                constraints: table =>
                {
                    table.PrimaryKey("PK_Boje", x => x.ID);
                    table.ForeignKey(
                        name: "FK_Boje_Modeli_ModelID",
                        column: x => x.ModelID,
                        principalTable: "Modeli",
                        principalColumn: "ID");
                });

            migrationBuilder.CreateTable(
                name: "Automobili",
                columns: table => new
                {
                    ID = table.Column<int>(type: "int", nullable: false)
                        .Annotation("SqlServer:Identity", "1, 1"),
                    SlikaPath = table.Column<string>(type: "nvarchar(max)", nullable: false),
                    Kolicina = table.Column<int>(type: "int", nullable: false),
                    Cena = table.Column<int>(type: "int", nullable: false),
                    DatumPoslednjeProdaje = table.Column<DateTime>(type: "datetime2", nullable: false),
                    MarkaID = table.Column<int>(type: "int", nullable: true),
                    ModelID = table.Column<int>(type: "int", nullable: true),
                    BojaID = table.Column<int>(type: "int", nullable: true)
                },
                constraints: table =>
                {
                    table.PrimaryKey("PK_Automobili", x => x.ID);
                    table.ForeignKey(
                        name: "FK_Automobili_Boje_BojaID",
                        column: x => x.BojaID,
                        principalTable: "Boje",
                        principalColumn: "ID");
                    table.ForeignKey(
                        name: "FK_Automobili_Marke_MarkaID",
                        column: x => x.MarkaID,
                        principalTable: "Marke",
                        principalColumn: "ID");
                    table.ForeignKey(
                        name: "FK_Automobili_Modeli_ModelID",
                        column: x => x.ModelID,
                        principalTable: "Modeli",
                        principalColumn: "ID");
                });

            migrationBuilder.CreateIndex(
                name: "IX_Automobili_BojaID",
                table: "Automobili",
                column: "BojaID");

            migrationBuilder.CreateIndex(
                name: "IX_Automobili_MarkaID",
                table: "Automobili",
                column: "MarkaID");

            migrationBuilder.CreateIndex(
                name: "IX_Automobili_ModelID",
                table: "Automobili",
                column: "ModelID");

            migrationBuilder.CreateIndex(
                name: "IX_Boje_ModelID",
                table: "Boje",
                column: "ModelID");

            migrationBuilder.CreateIndex(
                name: "IX_Modeli_MarkaID",
                table: "Modeli",
                column: "MarkaID");
        }

        /// <inheritdoc />
        protected override void Down(MigrationBuilder migrationBuilder)
        {
            migrationBuilder.DropTable(
                name: "Automobili");

            migrationBuilder.DropTable(
                name: "Boje");

            migrationBuilder.DropTable(
                name: "Modeli");

            migrationBuilder.DropTable(
                name: "Marke");
        }
    }
}
