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
                name: "Stanovi",
                columns: table => new
                {
                    ID = table.Column<int>(type: "int", nullable: false)
                        .Annotation("SqlServer:Identity", "1, 1"),
                    ImeVlasnika = table.Column<string>(type: "nvarchar(30)", maxLength: 30, nullable: false),
                    Povrsina = table.Column<int>(type: "int", nullable: false),
                    BrClanova = table.Column<int>(type: "int", nullable: false)
                },
                constraints: table =>
                {
                    table.PrimaryKey("PK_Stanovi", x => x.ID);
                });

            migrationBuilder.CreateTable(
                name: "Racuni",
                columns: table => new
                {
                    ID = table.Column<int>(type: "int", nullable: false)
                        .Annotation("SqlServer:Identity", "1, 1"),
                    MesecIzdavanja = table.Column<int>(type: "int", nullable: false),
                    Voda = table.Column<int>(type: "int", nullable: false),
                    Struja = table.Column<int>(type: "int", nullable: false),
                    KomunalneUsluge = table.Column<int>(type: "int", nullable: false),
                    Placen = table.Column<string>(type: "nvarchar(2)", maxLength: 2, nullable: false),
                    StanID = table.Column<int>(type: "int", nullable: true)
                },
                constraints: table =>
                {
                    table.PrimaryKey("PK_Racuni", x => x.ID);
                    table.ForeignKey(
                        name: "FK_Racuni_Stanovi_StanID",
                        column: x => x.StanID,
                        principalTable: "Stanovi",
                        principalColumn: "ID");
                });

            migrationBuilder.CreateIndex(
                name: "IX_Racuni_StanID",
                table: "Racuni",
                column: "StanID");
        }

        /// <inheritdoc />
        protected override void Down(MigrationBuilder migrationBuilder)
        {
            migrationBuilder.DropTable(
                name: "Racuni");

            migrationBuilder.DropTable(
                name: "Stanovi");
        }
    }
}
