using System;
using Microsoft.EntityFrameworkCore.Migrations;

#nullable disable

namespace WebTemplate.Migrations
{
    /// <inheritdoc />
    public partial class V1 : Migration
    {
        /// <inheritdoc />
        protected override void Up(MigrationBuilder migrationBuilder)
        {
            migrationBuilder.CreateTable(
                name: "Autori",
                columns: table => new
                {
                    ID = table.Column<int>(type: "int", nullable: false)
                        .Annotation("SqlServer:Identity", "1, 1"),
                    Ime = table.Column<string>(type: "nvarchar(max)", nullable: false),
                    Prezime = table.Column<string>(type: "nvarchar(max)", nullable: false),
                    DatumRodjenja = table.Column<DateTime>(type: "datetime2", nullable: false),
                    PrviAlbum = table.Column<DateTime>(type: "datetime2", nullable: true)
                },
                constraints: table =>
                {
                    table.PrimaryKey("PK_Autori", x => x.ID);
                });

            migrationBuilder.CreateTable(
                name: "Albumi",
                columns: table => new
                {
                    ID = table.Column<int>(type: "int", nullable: false)
                        .Annotation("SqlServer:Identity", "1, 1"),
                    Naziv = table.Column<string>(type: "nvarchar(max)", nullable: false),
                    GodinaIzdavanja = table.Column<DateTime>(type: "datetime2", nullable: false),
                    IzdavackaKuca = table.Column<string>(type: "nvarchar(max)", nullable: false),
                    AutorID = table.Column<int>(type: "int", nullable: true)
                },
                constraints: table =>
                {
                    table.PrimaryKey("PK_Albumi", x => x.ID);
                    table.ForeignKey(
                        name: "FK_Albumi_Autori_AutorID",
                        column: x => x.AutorID,
                        principalTable: "Autori",
                        principalColumn: "ID");
                });

            migrationBuilder.CreateTable(
                name: "Numere",
                columns: table => new
                {
                    ID = table.Column<int>(type: "int", nullable: false)
                        .Annotation("SqlServer:Identity", "1, 1"),
                    Naziv = table.Column<string>(type: "nvarchar(max)", nullable: false),
                    Trajanje = table.Column<TimeSpan>(type: "time", nullable: false),
                    Zanr = table.Column<string>(type: "nvarchar(max)", nullable: false),
                    BrojUmetnika = table.Column<long>(type: "bigint", nullable: false),
                    AlbumID = table.Column<int>(type: "int", nullable: true)
                },
                constraints: table =>
                {
                    table.PrimaryKey("PK_Numere", x => x.ID);
                    table.ForeignKey(
                        name: "FK_Numere_Albumi_AlbumID",
                        column: x => x.AlbumID,
                        principalTable: "Albumi",
                        principalColumn: "ID");
                });

            migrationBuilder.CreateIndex(
                name: "IX_Albumi_AutorID",
                table: "Albumi",
                column: "AutorID");

            migrationBuilder.CreateIndex(
                name: "IX_Numere_AlbumID",
                table: "Numere",
                column: "AlbumID");
        }

        /// <inheritdoc />
        protected override void Down(MigrationBuilder migrationBuilder)
        {
            migrationBuilder.DropTable(
                name: "Numere");

            migrationBuilder.DropTable(
                name: "Albumi");

            migrationBuilder.DropTable(
                name: "Autori");
        }
    }
}
