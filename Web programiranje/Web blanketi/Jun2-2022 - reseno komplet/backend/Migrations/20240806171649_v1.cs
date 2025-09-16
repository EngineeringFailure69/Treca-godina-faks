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
                name: "Kompanije",
                columns: table => new
                {
                    ID = table.Column<int>(type: "int", nullable: false)
                        .Annotation("SqlServer:Identity", "1, 1"),
                    Naziv = table.Column<string>(type: "nvarchar(50)", maxLength: 50, nullable: false),
                    ProsecnaZarada = table.Column<int>(type: "int", nullable: false),
                    Cena = table.Column<int>(type: "int", nullable: false),
                    BrojDanaZaIsporuku = table.Column<int>(type: "int", nullable: false)
                },
                constraints: table =>
                {
                    table.PrimaryKey("PK_Kompanije", x => x.ID);
                });

            migrationBuilder.CreateTable(
                name: "Vozila",
                columns: table => new
                {
                    ID = table.Column<int>(type: "int", nullable: false)
                        .Annotation("SqlServer:Identity", "1, 1"),
                    Naziv = table.Column<string>(type: "nvarchar(50)", maxLength: 50, nullable: false),
                    SlikaPath = table.Column<string>(type: "nvarchar(max)", nullable: false),
                    Zapremina = table.Column<int>(type: "int", nullable: false),
                    Tezina = table.Column<int>(type: "int", nullable: false),
                    Cena = table.Column<int>(type: "int", nullable: false),
                    DatumPrijema = table.Column<DateTime>(type: "datetime2", nullable: false),
                    DatumIsporuke = table.Column<DateTime>(type: "datetime2", nullable: false),
                    KompanijinoVoziloID = table.Column<int>(type: "int", nullable: true)
                },
                constraints: table =>
                {
                    table.PrimaryKey("PK_Vozila", x => x.ID);
                    table.ForeignKey(
                        name: "FK_Vozila_Kompanije_KompanijinoVoziloID",
                        column: x => x.KompanijinoVoziloID,
                        principalTable: "Kompanije",
                        principalColumn: "ID");
                });

            migrationBuilder.CreateTable(
                name: "Roba",
                columns: table => new
                {
                    ID = table.Column<int>(type: "int", nullable: false)
                        .Annotation("SqlServer:Identity", "1, 1"),
                    Zapremina = table.Column<int>(type: "int", nullable: false),
                    Tezina = table.Column<int>(type: "int", nullable: false),
                    CenaOd = table.Column<int>(type: "int", nullable: false),
                    CenaDo = table.Column<int>(type: "int", nullable: false),
                    DatumPrijema = table.Column<DateTime>(type: "datetime2", nullable: false),
                    DatumIsporuke = table.Column<DateTime>(type: "datetime2", nullable: false),
                    VoziloID = table.Column<int>(type: "int", nullable: true)
                },
                constraints: table =>
                {
                    table.PrimaryKey("PK_Roba", x => x.ID);
                    table.ForeignKey(
                        name: "FK_Roba_Vozila_VoziloID",
                        column: x => x.VoziloID,
                        principalTable: "Vozila",
                        principalColumn: "ID");
                });

            migrationBuilder.CreateIndex(
                name: "IX_Roba_VoziloID",
                table: "Roba",
                column: "VoziloID");

            migrationBuilder.CreateIndex(
                name: "IX_Vozila_KompanijinoVoziloID",
                table: "Vozila",
                column: "KompanijinoVoziloID");
        }

        /// <inheritdoc />
        protected override void Down(MigrationBuilder migrationBuilder)
        {
            migrationBuilder.DropTable(
                name: "Roba");

            migrationBuilder.DropTable(
                name: "Vozila");

            migrationBuilder.DropTable(
                name: "Kompanije");
        }
    }
}
