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
                name: "Gradovi",
                columns: table => new
                {
                    ID = table.Column<int>(type: "int", nullable: false)
                        .Annotation("SqlServer:Identity", "1, 1"),
                    Naziv = table.Column<string>(type: "nvarchar(max)", nullable: false),
                    Latitude = table.Column<double>(type: "float", nullable: false),
                    Longitude = table.Column<double>(type: "float", nullable: false),
                    BrojKoloseka = table.Column<long>(type: "bigint", nullable: false),
                    BrojStanovnika = table.Column<long>(type: "bigint", nullable: false)
                },
                constraints: table =>
                {
                    table.PrimaryKey("PK_Gradovi", x => x.ID);
                });

            migrationBuilder.CreateTable(
                name: "Vozovi",
                columns: table => new
                {
                    ID = table.Column<int>(type: "int", nullable: false)
                        .Annotation("SqlServer:Identity", "1, 1"),
                    MaxKapacitetPutnika = table.Column<long>(type: "bigint", nullable: false),
                    DatumProizvodnje = table.Column<DateTime>(type: "datetime2", nullable: false),
                    MaxBrzina = table.Column<long>(type: "bigint", nullable: false),
                    Tezina = table.Column<long>(type: "bigint", nullable: false)
                },
                constraints: table =>
                {
                    table.PrimaryKey("PK_Vozovi", x => x.ID);
                });

            migrationBuilder.CreateTable(
                name: "Relacije",
                columns: table => new
                {
                    ID = table.Column<int>(type: "int", nullable: false)
                        .Annotation("SqlServer:Identity", "1, 1"),
                    BrojPutnika = table.Column<long>(type: "bigint", nullable: false),
                    CenaKarte = table.Column<long>(type: "bigint", nullable: false),
                    Datum = table.Column<DateTime>(type: "datetime2", nullable: false),
                    GradPolaskaID = table.Column<int>(type: "int", nullable: true),
                    GradDolaskaID = table.Column<int>(type: "int", nullable: true),
                    VozID = table.Column<int>(type: "int", nullable: true)
                },
                constraints: table =>
                {
                    table.PrimaryKey("PK_Relacije", x => x.ID);
                    table.ForeignKey(
                        name: "FK_Relacije_Gradovi_GradDolaskaID",
                        column: x => x.GradDolaskaID,
                        principalTable: "Gradovi",
                        principalColumn: "ID");
                    table.ForeignKey(
                        name: "FK_Relacije_Gradovi_GradPolaskaID",
                        column: x => x.GradPolaskaID,
                        principalTable: "Gradovi",
                        principalColumn: "ID");
                    table.ForeignKey(
                        name: "FK_Relacije_Vozovi_VozID",
                        column: x => x.VozID,
                        principalTable: "Vozovi",
                        principalColumn: "ID");
                });

            migrationBuilder.CreateIndex(
                name: "IX_Relacije_GradDolaskaID",
                table: "Relacije",
                column: "GradDolaskaID");

            migrationBuilder.CreateIndex(
                name: "IX_Relacije_GradPolaskaID",
                table: "Relacije",
                column: "GradPolaskaID");

            migrationBuilder.CreateIndex(
                name: "IX_Relacije_VozID",
                table: "Relacije",
                column: "VozID");
        }

        /// <inheritdoc />
        protected override void Down(MigrationBuilder migrationBuilder)
        {
            migrationBuilder.DropTable(
                name: "Relacije");

            migrationBuilder.DropTable(
                name: "Gradovi");

            migrationBuilder.DropTable(
                name: "Vozovi");
        }
    }
}
