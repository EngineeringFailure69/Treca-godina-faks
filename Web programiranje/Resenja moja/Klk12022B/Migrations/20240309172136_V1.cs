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
                name: "Distribucije",
                columns: table => new
                {
                    ID = table.Column<int>(type: "int", nullable: false)
                        .Annotation("SqlServer:Identity", "1, 1"),
                    Naziv = table.Column<string>(type: "nvarchar(50)", maxLength: 50, nullable: false),
                    Grad = table.Column<string>(type: "nvarchar(50)", maxLength: 50, nullable: false),
                    Email = table.Column<string>(type: "nvarchar(50)", maxLength: 50, nullable: false),
                    BrojRadnika = table.Column<long>(type: "bigint", nullable: false)
                },
                constraints: table =>
                {
                    table.PrimaryKey("PK_Distribucije", x => x.ID);
                });

            migrationBuilder.CreateTable(
                name: "Potrosaci",
                columns: table => new
                {
                    ID = table.Column<int>(type: "int", nullable: false)
                        .Annotation("SqlServer:Identity", "1, 1"),
                    Ime = table.Column<string>(type: "nvarchar(50)", maxLength: 50, nullable: false),
                    Prezime = table.Column<string>(type: "nvarchar(50)", maxLength: 50, nullable: false),
                    GodinaRodjenja = table.Column<long>(type: "bigint", nullable: false),
                    MestoRodjenja = table.Column<string>(type: "nvarchar(max)", nullable: false)
                },
                constraints: table =>
                {
                    table.PrimaryKey("PK_Potrosaci", x => x.ID);
                });

            migrationBuilder.CreateTable(
                name: "Podrucja",
                columns: table => new
                {
                    ID = table.Column<int>(type: "int", nullable: false)
                        .Annotation("SqlServer:Identity", "1, 1"),
                    KorisnickiBroj = table.Column<long>(type: "bigint", nullable: false),
                    DatumPotpisivanjaUgovora = table.Column<DateTime>(type: "datetime2", nullable: false),
                    BrojBrojila = table.Column<string>(type: "nvarchar(max)", nullable: false),
                    DistribucijaID = table.Column<int>(type: "int", nullable: true),
                    PotrosacID = table.Column<int>(type: "int", nullable: true)
                },
                constraints: table =>
                {
                    table.PrimaryKey("PK_Podrucja", x => x.ID);
                    table.ForeignKey(
                        name: "FK_Podrucja_Distribucije_DistribucijaID",
                        column: x => x.DistribucijaID,
                        principalTable: "Distribucije",
                        principalColumn: "ID");
                    table.ForeignKey(
                        name: "FK_Podrucja_Potrosaci_PotrosacID",
                        column: x => x.PotrosacID,
                        principalTable: "Potrosaci",
                        principalColumn: "ID");
                });

            migrationBuilder.CreateIndex(
                name: "IX_Podrucja_DistribucijaID",
                table: "Podrucja",
                column: "DistribucijaID");

            migrationBuilder.CreateIndex(
                name: "IX_Podrucja_PotrosacID",
                table: "Podrucja",
                column: "PotrosacID");
        }

        /// <inheritdoc />
        protected override void Down(MigrationBuilder migrationBuilder)
        {
            migrationBuilder.DropTable(
                name: "Podrucja");

            migrationBuilder.DropTable(
                name: "Distribucije");

            migrationBuilder.DropTable(
                name: "Potrosaci");
        }
    }
}
